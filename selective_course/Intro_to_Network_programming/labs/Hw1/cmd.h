#include <iostream>
#include <string>
#include <vector>
#include <utility>

using namespace std;

string gen(string no, string username, vector<string> params){
    string _ret = ":mircd " + no + " " + username;
    params[params.size()-1] = ":" + params[params.size()-1];
    for(auto &i : params){
        _ret += " " + i;
    }
    _ret += "\r\n";
    return _ret;
}

string _001(string username){
    return gen("001", username, {"Welcome to the minimized IRC daemon!"});
}

string _321(string username){
    return gen("321", username, {"Channel", "Users Name"});
}

string _322(string username, string channel_name, bool visible, string topic){
    return gen("322", username, {channel_name, visible?"1":"0", topic});
}

string _323(string username){
    return gen("323", username, {"End of Liset"});
}

string _331(string username, string channel){
    return gen("331", username, {channel, "No topic is set"});
}

string _332(string username, string channel, string topic){
    return gen("332", username, {channel, topic});
}

string _353(string username, string channel, vector<string> users){
    string u;
    for(int i = 0;i<users.size();i++){
        if(i!=users.size()-1){
            u += users[i]+' ';
        }
        else{
            u += users[i];
        }
    }
    return gen("353", username, {channel, u});
}

string _366(string username, string channel){
    return gen("366", username, {channel, "End of Names List"});
}

string _372(string username){
    string _ret;
    _ret += gen("372", username, {"-  Hello, World!"});
    _ret += gen("372", username, {"-               @                    _ "});
    _ret += gen("372", username, {"-   ____  ___   _   _ _   ____.     | |"});
    _ret += gen("372", username, {"-  /  _ `'_  \\ | | | '_/ /  __|  ___| |"});
    _ret += gen("372", username, {"-  | | | | | | | | | |   | |    /  _  |"});
    _ret += gen("372", username, {"-  | | | | | | | | | |   | |__  | |_| |"});
    _ret += gen("372", username, {"-  |_| |_| |_| |_| |_|   \\____| \\___,_|"});
    _ret += gen("372", username, {"-  minimized internet relay chat daemon"});
    _ret += gen("372", username, {"-"});
    return _ret;
}

string _375(string username){
    return gen("375", username, {"- mircd Message of the day -"});
}

string _376(string username){
    return gen("376", username, {"End of message of the day"});
}

string _392(string username){
    return gen("392", username, {"UserID   Terminal  Host"});
}

string _393(string username, vector<pair<string, string>> users){
    string _ret;
    for(auto &i: users){
        string name = i.first;
        string ip = i.second;
        string terminal = " -        ";
        while(name.length()<8) name.push_back(' ');
        while(ip.length()<8) ip.push_back(' ');
        _ret += gen("393", username, {name+terminal+ip});
    }
    return _ret;
}

string _394(string username){
    return gen("394", username, {"End of users"});
}

string _401(string username, string nickname){
    return gen("401", username, {nickname, "No such nick/channel"});
}

string _403(string username, string channelName){
    return gen("403", username, {channelName, "No such channel"});
}

string _409(string username){
    return gen("409", username, {"No origin specified"});
}

string _411(string username, string cmd){
    return gen("411", username, {"No recipient given ("+cmd+")"});
}

string _412(string username){
    return gen("412", username, {"No text to send"});
}

string _421(string username, string cmd){
    return gen("421", username, {cmd, "Unknown command"});
}

string _431(){
    return ":micrd 431 :No nickname given\r\n";
}

string _436(string username){
    return gen("436", username, {"Nickname collision KILL"});
}

string _442(string username, string channelName){
    return gen("442", username, {channelName, "You are not on that channel"});
}

string _461(string username, string cmd){
    return gen("461", username, {cmd, "Not enough parameters"});
}