/* 
	Copyright 2010 OpenRTMFP
 
	This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License received along this program for more
	details (or else see http://www.gnu.org/licenses/).

	This file is a part of Cumulus.
*/

#pragma once

#include "Cumulus.h"
#include "Session.h"
#include "PacketReader.h"
#include "Handshake.h"
#include "ServerHandler.h"
#include "Gateway.h"
#include "Sessions.h"
#include "Poco/Runnable.h"
#include "Poco/Mutex.h"
#include "Poco/Thread.h"
#include "Poco/Net/DatagramSocket.h"
#include "Poco/Net/SocketAddress.h"


namespace Cumulus {

class RTMFPServerParams {
public:
	RTMFPServerParams() : port(RTMFP_DEFAULT_PORT),pCirrus(NULL),middle(false) {
	}
	Poco::UInt16				port;
	bool						middle;
	Poco::Net::SocketAddress*	pCirrus;
};

class CUMULUS_API RTMFPServer : public Poco::Runnable,private Gateway {
public:
	RTMFPServer(Poco::UInt8 keepAliveServer=15,Poco::UInt8 keepAlivePeer=10);
	RTMFPServer(ClientHandler& clientHandler,Poco::UInt8 keepAliveServer=15,Poco::UInt8 keepAlivePeer=10);
	virtual ~RTMFPServer();

	void start(RTMFPServerParams& params);
	void stop();
	bool running();

private:
	Session* findSession(Poco::UInt32 id);
	void	 run();
	Poco::UInt8		p2pHandshake(const std::string& tag,PacketWriter& response,const Poco::Net::SocketAddress& address,const Poco::UInt8* peerIdWanted);
	Poco::UInt32	createSession(Poco::UInt32 farId,const Peer& peer,const Poco::UInt8* decryptKey,const Poco::UInt8* encryptKey,Cookie& cookie);
	void			manage();

	Handshake					_handshake;

	volatile bool				_terminate;
	Poco::FastMutex				_mutex;
	Poco::UInt16				_port;
	Poco::Thread				_mainThread;
	Poco::Net::DatagramSocket	_socket;

	bool							_middle;
	Target*							_pCirrus;
	ServerHandler					_handler;
	Sessions						_sessions;
	Poco::UInt32					_nextIdSession;

	Poco::Timestamp					_timeLastManage;
	Poco::UInt32					_freqManage;
};

inline bool RTMFPServer::running() {
	return _mainThread.isRunning();
}


} // namespace Cumulus
