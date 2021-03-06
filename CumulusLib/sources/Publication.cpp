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

#include "Publication.h"
#include "Poco/StreamCopier.h"
#include "Logs.h"

using namespace std;
using namespace Poco;


namespace Cumulus {

Publication::Publication():publisherId(0) {
	
}


Publication::~Publication() {
}

void Publication::pushRawPacket(UInt8 type,PacketReader& packet) {
	list<Listener*>::const_iterator it;
	int pos = packet.position();
	for(it=_listeners.begin();it!=_listeners.end();++it) {
		(*it)->pushRawPacket(type,packet);
		packet.reset(pos);
	}
}

void Publication::pushAudioPacket(PacketReader& packet) {
	UInt32 time = packet.read32(); // TODO?
	list<Listener*>::const_iterator it;
	int pos = packet.position();
	for(it=_listeners.begin();it!=_listeners.end();++it) {
		(*it)->pushAudioPacket(packet);
		packet.reset(pos);
	}
}

void Publication::pushVideoPacket(PacketReader& packet) {
	UInt32 time = packet.read32(); // TODO?
	list<Listener*>::const_iterator it;
	int pos = packet.position();
	for(it=_listeners.begin();it!=_listeners.end();++it) {
		(*it)->pushVideoPacket(packet);
		packet.reset(pos);
	}
}

void Publication::add(Listener& listener) {
	list<Listener*>::const_iterator it;
	for(it=_listeners.begin();it!=_listeners.end();++it) {
		if((&listener)==*it)
			return;
	}
	_listeners.push_back(&listener);
}

void Publication::remove(Listener& listener) {
	list<Listener*>::iterator it;
	for(it=_listeners.begin();it!=_listeners.end();++it) {
		if((&listener)==*it) {
			_listeners.erase(it);
			return;
		}
	}
}



} // namespace Cumulus
