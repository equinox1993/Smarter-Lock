//
//  REPL.cpp
//  Smarter Lock
//
//  Created by Yuwei Huang on 15/5/23.
//  Copyright (c) 2015年 CSE481. All rights reserved.
//

#include "REPL.h"

#include "ServerThreads.h"
#include "CameraLoop.h"
#include "APNClient.h"

#include <sstream>
#include <iostream>
#include <vector>

using namespace std;

CleanupFunc REPL::cleanupFn;
const char* REPL::password;

const char HELP_STRING[] =
	"Commands:\n"
	"\tunlock PSW    Unlock using the password PSW\n"
	"\tqrdec LOOPS   Open camera and decode QR for LOOPS*CAM_WAIT msecs\n"
	"\tring          Ring the doorbell (Send notification to clients).\n"
	"\thelp          Show this help\n"
	"\tquit          Quit server\n"
	;

// returns: command name
// will fill args
string parseline(const string& str, vector<string>& args) {
	istringstream f(str);
	string s;
	string cmd;
	while (getline(f, s, ' ')) {
		if (cmd.empty())
			cmd = s;
		else {
			args.push_back(s);
		}
	}
	
	return cmd;
}

void exec(const string& cmd, const vector<string>& args) {
	if (cmd == "help") {
		cout<<HELP_STRING;
	} else if (cmd == "quit" || cmd == "exit") {
		REPL::cleanupFn();
	} else if (cmd == "unlock") {
		string psw;
		if (args.size() > 0)
			psw = args[0];
		
		if (psw == REPL::password) {
			ServerThreads::unlockDoor();
			cout<<"Door unlocked\n";
		} else {
			cout<<"Incorrect password\n";
		}
	} else if (cmd == "qrdec") {
		if (args.size() < 1) {
			cout<<"qrdec: wrong arguments\n";
			return;
		}
		
		int wait = atoi(args[0].c_str());
		CameraLoop::decQrWait = wait;
		
		cout << "Will detect QR code for "<<wait<<" loops.\n";
	} else if (cmd == "ring") {
		if (!APNClient::DefaultClient.connect()) {
			cout<<"Failed to connect to APN server."<<endl;
			return;
		}
		
		cout << "Sending notifications to devices...\nTokens:\n";
		for (auto it = ServerThreads::devices.begin(); it != ServerThreads::devices.end(); it++) {
			string token = *it;
			
			cout << token << endl;
			APNClient::DefaultClient.notify(token);
		}
		
		cout << "Done\n";
	} else {
		cout<<"Unknown command "<<cmd<<".\n\n"<<HELP_STRING;
	}
}

void REPL::loop() {
	while (true) {
		string line;
		getline(cin, line);
		
		if (line.empty() || line[0]=='#')
			continue;
		
		vector<string> args;
		string cmd = parseline(line, args);
		
		exec(cmd, args);
	}
}

void* REPL::startLoop(void* sth) {
	loop();
	return nullptr;
}