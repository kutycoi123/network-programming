/**
* Name: TRUNGLAM NGUYEN
* Email: tln3@sfu.ca
* */
package rdt;

import java.io.*;
import java.net.*;
import java.util.*;
import java.util.Timer;
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

		int curr = 0;
		int maximumDataLength = MSS - RDTSegment.HDR_SIZE;
		while (curr < size) {
			int next = Math.min(curr + maximumDataLength, size);
			RDTSegment segment = new RDTSegment();
			for (int pos = curr; pos < next; ++pos) {
				segment.data[pos-curr] = data[pos];
			}
			segment.length = next - curr;
			sndBuf.putNext(segment);
			segment.genChecksum();
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
		RDTSegment seg = null;
		while (seg == null) {
			seg = rcvBuf.getNext();
			// Received client shutdown segment => reset the buffer and ready for new client connection
			if (seg.flags == RDTSegment.FLAGS_CLIENT_SHUTDOWN) {
				System.out.println("Client has already closed. Server is about to reset");
				rcvBuf.popNext();
				rcvBuf.reset();
				seg = null;
			}
		}
		seg.makePayload(buf);
		rcvBuf.popNext();
		return seg.length + RDTSegment.HDR_SIZE;   // fix
	}
	
	// called by app
	public void close() {
		// OPTIONAL: close the connection gracefully
		// you can use TCP-style connection termination process
		// Send a client shutdown segment and put this segment into sndBuf
		RDTSegment shutdownSeg = new RDTSegment();
		shutdownSeg.flags = RDTSegment.FLAGS_CLIENT_SHUTDOWN;
		sndBuf.putNext(shutdownSeg);
		shutdownSeg.genChecksum();
		// Wait a bit for all data in sndBuf to be successfully received (acknowledged)
		try {
			Thread.sleep((sndBuf.next - sndBuf.base) * RTO);
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
		Utility.udp_send(shutdownSeg,socket,dst_ip,dst_port);

		// schedule timeout for shutdown segment
		TimeoutHandler handler = new TimeoutHandler(sndBuf,shutdownSeg,socket,dst_ip,dst_port);
		timer.schedule(handler, 0, RTO);
		while(true) {
			try {
				sndBuf.semEmpty.acquire();
				sndBuf.semMutex.acquire();
				if (sndBuf.base == sndBuf.next) { // sndBuf is totally empty, meaning server has received all segments
					// Time to close socket and timer
					socket.close(); // This one will cause receiver thread throw exception, therefore, terminated
					timer.cancel();
					break;
				}
				sndBuf.semMutex.release();
				sndBuf.semEmpty.release();

			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
	}
	
}  // end RDT class 


class RDTBuffer {
	public RDTSegment[] buf;
	public int size;	
	public int base;
	public int next;
	public int oldBase; // The previous in-order sequence number
	public boolean clientAboutToShutDown;
	public boolean serverAboutToShutdown;
	public Semaphore semMutex; // for mutual execlusion
	public Semaphore semFull; // #of full slots
	public Semaphore semEmpty; // #of Empty slots
	
	RDTBuffer (int bufSize) {
		buf = new RDTSegment[bufSize];
		for (int i=0; i<bufSize; i++)
			buf[i] = null;
		size = bufSize;
		base = next = 0;
		oldBase = -1;
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
				seg.seqNum = next;
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
		
		return null;
	}

	//Remove segment at base
	public RDTSegment popNext() {
		try {
			semFull.acquire(); // only pop when there is something in buffer
			semMutex.acquire();
			if (buf[base%size] != null && buf[base%size].seqNum == base) {
				oldBase = base;
				base++;
				semEmpty.release(); // Increase empty slot by 1
			} else {
				semFull.release();
			}
			RDTSegment seg = buf[(base-1)%size];
			semMutex.release();
			return seg;
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
		return null;
	}
	// Check if seqNum is within buffer window [base, base + size - 1]
	public boolean isWithinBufferWindow(int seqNum) {
		return seqNum >= base && seqNum < base + size;
	}
	// Get segment with seqNum (not index)
	public RDTSegment getSeg(int seqNum) {
		if (!isWithinBufferWindow(seqNum)) return null;
		return buf[seqNum % size];
	}
	// Put a segment in the *right* slot based on seg.seqNum
	// used by receiver in Selective Repeat
	public void putSeqNum (RDTSegment seg) {
		// ***** complete
		try {
			semEmpty.acquire(); // wait for an empty slot
			semMutex.acquire(); // wait for mutex
			if (isWithinBufferWindow(seg.seqNum)){
				if (buf[seg.seqNum % size] == null || buf[seg.seqNum % size].seqNum != seg.seqNum){
					buf[seg.seqNum % size] = seg;
					next = Math.max(next, seg.seqNum + 1);
					semFull.release(); // increase #of full slots
				} else {
					buf[seg.seqNum % size] = seg;
				}
			} else {
				semEmpty.release();
			}
			semMutex.release();
		} catch(Exception e) {
			e.printStackTrace();
		}

	}
	// Reset the buffer
	public void reset() {
		try {
			while (true) {
				semEmpty.acquire();
				semMutex.acquire();
				if (base == next) {
					oldBase = base - 1;
					base = next = 0;
					buf = new RDTSegment[size];
					for (int i = 0; i < size; i++)
						buf[i] = null;
					semMutex.release();
					semEmpty.release();
					break;
				}
				semMutex.release();
				semEmpty.release();
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
	// Slide the buffer window
	public void slide() {
		try {
			semMutex.acquire();
			while (base < next && buf[base % size].ackReceived) {
				base++;
				semEmpty.release();
				semFull.acquire();
			}
			semMutex.release();
		} catch (Exception e) {
			e.printStackTrace();
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

				// Corrupted segment
				if (!seg.isValid()) {
					System.out.println("Segment is discarded due to corruption");
					continue;
				}

				switch (RDT.protocol) {
					case RDT.GBN:
						if (seg.containsAck()) {
							sndBuf.semMutex.acquire();
							if (sndBuf.isWithinBufferWindow(seg.ackNum)) {
								for (int i = sndBuf.base; i <= seg.ackNum; ++i) {
									sndBuf.getSeg(i).setAckReceived(true);
								}
							}
							sndBuf.semMutex.release();
							// Ask sndBuf to slide its buf (slide window)
							sndBuf.slide();
						} else if (seg.containsData() || seg.containsClientShutdownFlag()) {
							RDTSegment ackSeg = new RDTSegment();
							ackSeg.flags = RDTSegment.FLAGS_ACK;
							rcvBuf.semMutex.acquire();

							if (seg.seqNum == rcvBuf.base) {
								if (seg.seqNum == 0) { // New client, reset oldBase
									rcvBuf.oldBase = -1;
								}
								ackSeg.ackNum = seg.seqNum;
								rcvBuf.semMutex.release();
								rcvBuf.putNext(seg);
							} else {
								ackSeg.ackNum = rcvBuf.oldBase; // The latest in-order seqNum
								rcvBuf.semMutex.release();
							}
							ackSeg.genChecksum();
							Utility.udp_send(ackSeg, socket, dst_ip, dst_port);
						}
						break;
					case RDT.SR:
						if (seg.containsAck()) {
							sndBuf.semMutex.acquire();
							for (int i = 0; i < sndBuf.buf.length; ++i) {
								if (sndBuf.buf[i] != null && sndBuf.buf[i].seqNum == seg.ackNum) {
									sndBuf.buf[i].ackReceived = true;
									break;
								}
							}
							sndBuf.semMutex.release();
							// Ask sndBuf to slide its buf (slide window)
							sndBuf.slide();
						} else if (seg.containsData() || seg.containsClientShutdownFlag()) {
							rcvBuf.putSeqNum(seg);
							// Send ack
							RDTSegment ackSeg = new RDTSegment();
							ackSeg.ackNum = seg.seqNum;
							ackSeg.flags = RDTSegment.FLAGS_ACK;
							ackSeg.genChecksum();
							Utility.udp_send(ackSeg, socket, dst_ip, dst_port);
						}
						break;
				}

			} catch (Exception e) {
				System.out.println("Stopping receiver thread and close socket");
				break;
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

