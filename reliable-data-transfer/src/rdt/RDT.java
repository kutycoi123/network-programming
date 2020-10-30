
package rdt;

import javax.swing.text.Segment;
import java.io.*;
import java.net.*;
import java.util.*;
import java.util.concurrent.*;

public class RDT {

	public static final int MSS = 100; // Max segement size in bytes
	public static final int RTO = 500; // Retransmission Timeout in msec
	public static final int ERROR = -1;
	public static final int MAX_BUF_SIZE = 3;  
	public static final int GBN = 1;   // Go back N protocol
	public static final int SR = 2;    // Selective Repeat
	public static final int protocol = GBN;
	
	public static double lossRate = 0.0;
	public static Random random = new Random(); 
	public static Timer timer = new Timer();	
	
	private DatagramSocket socket; 
	private InetAddress dst_ip;
	private int dst_port;
	private int local_port; 
	
	private RDTBuffer sndBuf;
	private RDTBuffer rcvBuf;
	
	private ReceiverThread rcvThread;  
	
	
	RDT (String dst_hostname_, int dst_port_, int local_port_) 
	{
		local_port = local_port_;
		dst_port = dst_port_; 
		try {
			 socket = new DatagramSocket(local_port);
			 dst_ip = InetAddress.getByName(dst_hostname_);
		 } catch (IOException e) {
			 System.out.println("RDT constructor: " + e);
		 }
		sndBuf = new RDTBuffer(MAX_BUF_SIZE);
		if (protocol == GBN)
			rcvBuf = new RDTBuffer(1);
		else 
			rcvBuf = new RDTBuffer(MAX_BUF_SIZE);
		rcvThread = new ReceiverThread(rcvBuf, sndBuf, socket, dst_ip, dst_port);
		rcvThread.start();
	}
	
	RDT (String dst_hostname_, int dst_port_, int local_port_, int sndBufSize, int rcvBufSize)
	{
		local_port = local_port_;
		dst_port = dst_port_;
		 try {
			 socket = new DatagramSocket(local_port);
			 dst_ip = InetAddress.getByName(dst_hostname_);
		 } catch (IOException e) {
			 System.out.println("RDT constructor: " + e);
		 }
		sndBuf = new RDTBuffer(sndBufSize);
		if (protocol == GBN)
			rcvBuf = new RDTBuffer(1);
		else 
			rcvBuf = new RDTBuffer(rcvBufSize);
		
		rcvThread = new ReceiverThread(rcvBuf, sndBuf, socket, dst_ip, dst_port);
		rcvThread.start();
	}
	
	public static void setLossRate(double rate) {lossRate = rate;}
	
	// called by app
	// returns total number of sent bytes  
	public int send(byte[] data, int size) {
		
		//****** complete
		
		// divide data into segments
		// put each segment into sndBuf

		int curr = 0, seqNum = 0;
		while (curr < data.length) {
			int next = Math.min(curr + MSS, data.length);
			RDTSegment segment = new RDTSegment();
			for (int pos = curr; pos < next; ++pos) {
				segment.data[pos-curr] = data[pos];
			}
			segment.seqNum = seqNum++;
			sndBuf.putNext(segment);
			// send using udp_send()
			Utility.udp_send(segment,socket,dst_ip,dst_port);

			// schedule timeout for segment(s)
			TimeoutHandler handler = new TimeoutHandler(sndBuf,segment,socket,dst_ip,dst_port);
			timer.schedule(handler, 0, RTO);

			curr = next;
		}



		return size;
	}
	
	
	// called by app
	// receive one segment at a time
	// returns number of bytes copied in buf
	public int receive (byte[] buf, int size)
	{
		//*****  complete
		RDTSegment seg = rcvBuf.pop();
		seg.makePayload(buf);
		return seg.length + RDTSegment.HDR_SIZE;   // fix
	}
	
	// called by app
	public void close() {
		// OPTIONAL: close the connection gracefully
		// you can use TCP-style connection termination process
	}
	
}  // end RDT class 


class RDTBuffer {
	public RDTSegment[] buf;
	public int size;	
	public int base;
	public int next;
	public int expectedSeqNum;
	public Semaphore semMutex; // for mutual execlusion
	public Semaphore semFull; // #of full slots
	public Semaphore semEmpty; // #of Empty slots
	
	RDTBuffer (int bufSize) {
		buf = new RDTSegment[bufSize];
		for (int i=0; i<bufSize; i++)
			buf[i] = null;
		size = bufSize;
		base = next = 0;
		expectedSeqNum = 0;
		semMutex = new Semaphore(1, true);
		semFull =  new Semaphore(0, true);
		semEmpty = new Semaphore(bufSize, true);
	}

	
	
	// Put a segment in the next available slot in the buffer
	public void putNext(RDTSegment seg) {		
		try {
			semEmpty.acquire(); // wait for an empty slot 
			semMutex.acquire(); // wait for mutex
				buf[next%size] = seg;
				next++;
			semMutex.release();
			semFull.release(); // increase #of full slots
		} catch(InterruptedException e) {
			System.out.println("Buffer put(): " + e);
		}
	}
	
	// return the next in-order segment
	public RDTSegment getNext() {
		RDTSegment segment = null;
		// **** Complete
		try {
			semFull.acquire(); // only get when there is something in buffer
			semMutex.acquire();
			if (buf[base%size] != null && buf[base%size].seqNum == base) {
				segment = buf[base%size];
			}
			semMutex.release();
			semFull.release();
			return segment;
		} catch (InterruptedException e) {
			System.out.println("Buffer get(): " + e);
		}
		
		return null;  // fix
	}

	//Remove segment at base
	public RDTSegment pop() {
		try {
			semFull.acquire(); // only pop when there is something in buffer
			semMutex.acquire();
			base++;
			semMutex.release();
			semEmpty.release(); // Increase empty slot by 1
			return buf[(base-1)%size];
		} catch (InterruptedException e) {
			System.out.println("Buffer pop(): " + e);
		}
		return null;
	}
	
	// Put a segment in the *right* slot based on seg.seqNum
	// used by receiver in Selective Repeat
	public void putSeqNum (RDTSegment seg) {
		// ***** complete
		try {
			semEmpty.acquire(); // wait for an empty slot
			semMutex.acquire(); // wait for mutex
			if (seg.seqNum >= base || seg.seqNum < base + size) {
				buf[seg.seqNum % size] = seg;
			}
			semMutex.release();
			semFull.release(); // increase #of full slots
		} catch(InterruptedException e) {
			System.out.println("Buffer put(): " + e);
		}

	}
	
	// for debugging
	public void dump() {
		System.out.println("Dumping the receiver buffer ...");
		// Complete, if you want to 
		
	}
} // end RDTBuffer class



class ReceiverThread extends Thread {
	RDTBuffer rcvBuf, sndBuf;
	DatagramSocket socket;
	InetAddress dst_ip;
	int dst_port;
	
	ReceiverThread (RDTBuffer rcv_buf, RDTBuffer snd_buf, DatagramSocket s, 
			InetAddress dst_ip_, int dst_port_) {
		rcvBuf = rcv_buf;
		sndBuf = snd_buf;
		socket = s;
		dst_ip = dst_ip_;
		dst_port = dst_port_;
	}	
	public void run() {
		
		// *** complete 
		// Essentially:  while(cond==true){  // may loop for ever if you will not implement RDT::close()  
		//                socket.receive(pkt)
		//                seg = make a segment from the pkt
		//                verify checksum of seg
		//	              if seg contains ACK, process it potentailly removing segments from sndBuf
		//                if seg contains data, put the data in rcvBuf and do any necessary 
		//                             stuff (e.g, send ACK)
		//
		while(true) {
			byte[] buf = new byte[RDT.MSS];
			DatagramPacket pkt = new DatagramPacket(buf, buf.length);
			try {
				socket.receive(pkt);
				RDTSegment seg = new RDTSegment();
				makeSegment(seg, buf);
				if (!seg.isValid()) continue;
				if (seg.containsAck()) {
					for (int i = 0; i < sndBuf.buf.length; ++i) {
						if (sndBuf.buf[i].seqNum == seg.ackNum) {
							sndBuf.buf[i].ackReceived = true;
							// Maybe ask sndBuf to slide its buf (slide window)
							break;
						}
					}
				}
				if (seg.containsData()) {
					// Put seg in rcvBuf
					rcvBuf.putSeqNum(seg);
					RDTSegment ackSeg = new RDTSegment();
					ackSeg.ackNum = seg.seqNum;
					ackSeg.flags = RDTSegment.FLAGS_ACK;
					Utility.udp_send(ackSeg, socket, dst_ip, dst_port);
					// Send ack
				}

			} catch (IOException e) {
				System.out.println("ReceiverThread run(): " + e);
			}

		}
	}
	
	
//	 create a segment from received bytes 
	void makeSegment(RDTSegment seg, byte[] payload) {
	
		seg.seqNum = Utility.byteToInt(payload, RDTSegment.SEQ_NUM_OFFSET);
		seg.ackNum = Utility.byteToInt(payload, RDTSegment.ACK_NUM_OFFSET);
		seg.flags  = Utility.byteToInt(payload, RDTSegment.FLAGS_OFFSET);
		seg.checksum = Utility.byteToInt(payload, RDTSegment.CHECKSUM_OFFSET);
		seg.rcvWin = Utility.byteToInt(payload, RDTSegment.RCV_WIN_OFFSET);
		seg.length = Utility.byteToInt(payload, RDTSegment.LENGTH_OFFSET);
		//Note: Unlike C/C++, Java does not support explicit use of pointers! 
		// we have to make another copy of the data
		// This is not effecient in protocol implementation
		for (int i=0; i< seg.length; i++)
			seg.data[i] = payload[i + RDTSegment.HDR_SIZE]; 
	}
	
} // end ReceiverThread class

