/**
 * @author mhefeeda
 *
 */

package rdt;

import java.net.DatagramSocket;
import java.net.InetAddress;
import java.util.TimerTask;
	
class TimeoutHandler extends TimerTask {
	RDTBuffer sndBuf;
	RDTSegment seg; 
	DatagramSocket socket;
	InetAddress ip;
	int port;
	
	TimeoutHandler (RDTBuffer sndBuf_, RDTSegment s, DatagramSocket sock, 
			InetAddress ip_addr, int p) {
		sndBuf = sndBuf_;
		seg = s;
		socket = sock;
		ip = ip_addr;
		port = p;
	}

	@Override
	public void run() {
		
		System.out.println(System.currentTimeMillis()+ ":Timeout for seg: " + seg.seqNum);
		System.out.flush();
		
		// complete 
		switch(RDT.protocol){
			case RDT.GBN:
				try {
					sndBuf.semMutex.acquire();
					if (seg.seqNum == sndBuf.base && !seg.ackReceived) {
						// Resend all segments in sndBuf
						for (int i = 0; i < sndBuf.buf.length; ++i) {
							if (sndBuf.buf[i] != null) {
								Utility.udp_send(sndBuf.buf[i], socket, ip, port);
							}
						}
					} else if (seg.seqNum < sndBuf.base) {
						// This segment is already ACKed, timeout no needed.
						this.cancel();
					}
					sndBuf.semMutex.release();
				} catch (Exception e) {
					e.printStackTrace();
				}
				break;
			case RDT.SR:
				try {
					sndBuf.semMutex.acquire();
					if (!seg.ackReceived) {
						// Resend only this segment
						Utility.udp_send(seg, socket, ip, port);
					} else if (seg.seqNum < sndBuf.base) {
						// This segment is already  ACKed, timeout no needed.
						this.cancel();
					}
					sndBuf.semMutex.release();
				} catch (Exception e) {
					e.printStackTrace();
				}
				break;
			default:
				System.out.println("Error in TimeoutHandler:run(): unknown protocol");
		}
		
	}
} // end TimeoutHandler class

