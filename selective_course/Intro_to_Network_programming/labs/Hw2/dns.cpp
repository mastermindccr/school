#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <regex>

using namespace std;

typedef struct {
    /* byte 0, 1 */
    uint16_t ID      ;
    /* byte 2, a byte in big endian should be reversed*/
    uint16_t RD:    1;
    uint16_t TC:    1;
    uint16_t AA:    1;
    uint16_t Opcode:4;
    uint16_t QR:    1;
    /* byte 3, a byte in big endian should be reversed*/
    uint16_t RCODE: 4;
    uint16_t Z:     3;
    uint16_t RA:    1;
    /* byte 4-11 */
    uint16_t QDCount ;
    uint16_t ANCount ;
    uint16_t NSCount ;
    uint16_t ARCount ;
} Header_t;

typedef struct {
    vector<string> domain_pieces;
    string domain;
    uint16_t Qtype;
    uint16_t Qclass;
    void convert_domain(){
        for(int i = 0;i<domain_pieces.size();i++){
            domain+=domain_pieces[i];
            if(i!=domain_pieces.size()-1){
                domain+='.';
            }
        }
    }
} Question_t;

typedef struct {
    unsigned char name[100];
    unsigned char type[2];
    unsigned char clas[2];
    unsigned char ttl[4];
    unsigned char rdlength[2];
    unsigned char rdata[300];
    int name_len;
    int rdata_len;
} Record_t;

typedef struct {
    string sub;
    uint32_t ttl;
    string clas;
    string type;
    string params;
} domain;

string foreign;
vector<pair<string, vector<domain>>> local; // domain name to domain type

void parse_config(string filename){
    ifstream file;
    file.open(filename);
    string line;
    while(file>>line){
        stringstream ss(line);
        string str;
        vector<string> vec;
        while(getline(ss, str, ',')){
            vec.push_back(str);
        }
        if(vec.size()==2){ // local
            if(vec[0].back()=='.') vec[0].pop_back();

            ifstream file2;
            file2.open(vec[1]);
            string line;
            vector<domain> domains;
            getline(file2, line); // first line is domain
            while(getline(file2, line)){
                stringstream ss(line);
                string tmp;
                vector<string> strings;
                while(getline(ss, tmp, ',')){
                    strings.push_back(tmp);
                }
                domains.push_back({strings[0], (uint32_t)atol(strings[1].data()), strings[2], strings[3], strings[4]});
            }
            local.push_back(make_pair(vec[0], domains));

        }
        else{ // foreign
            foreign = vec[0];
        }
    }
}

int in_domain(string domain){
    for(int i = 0;i<local.size();i++){
        if(domain.find(local[i].first)!=-1) return i;
    }
    return -1;
}

vector<unsigned char> parse_domain_name(string domain){
    if(domain.back()=='\r') domain.pop_back();
    stringstream ss(domain);
    string line;
    vector<unsigned char> vec;
    while(getline(ss, line, '.')){
        vec.push_back((unsigned char)line.size());
        for(int i = 0;i<line.size();i++){
            vec.push_back(line[i]);
        }
    }
    vec.push_back((unsigned char)0);

    return vec;
}

int put_NS(vector<Record_t>& records, int file_index){
    int count = 0;
    for(auto &d: local[file_index].second){
        if(d.type=="NS"){
            count++;
            Record_t r;
            vector<unsigned char> vec = parse_domain_name(local[file_index].first);
            memcpy(r.name, &vec[0], vec.size());
            r.name_len = vec.size();
            uint16_t type = htons(2);
            uint16_t clas = htons(1);
            uint32_t ttl = htonl(d.ttl);
            memcpy(r.type, &type, sizeof(type));
            memcpy(r.clas, &clas, sizeof(clas));
            memcpy(r.ttl, &ttl, sizeof(ttl));

            vector<unsigned char> vec2 = parse_domain_name(d.params);
            uint16_t rdlength = htons(vec2.size());
            memcpy(r.rdlength, &rdlength, sizeof(rdlength));
            memcpy(r.rdata, &vec2[0], vec2.size());
            r.rdata_len = vec2.size();
            records.push_back(r);
        }
    }
    return count;
}

int put_SOA(vector<Record_t>& records, int file_index){
    for(auto &d: local[file_index].second){
        if(d.type=="SOA"){
            Record_t r;
            vector<unsigned char> vec = parse_domain_name(local[file_index].first);
            memcpy(r.name, &vec[0], vec.size());
            r.name_len = vec.size();
            uint16_t type = htons(6);
            uint16_t clas = htons(1);
            uint32_t ttl = htonl(d.ttl);
            memcpy(r.type, &type, sizeof(type));
            memcpy(r.clas, &clas, sizeof(clas));
            memcpy(r.ttl, &ttl, sizeof(ttl));

            stringstream ss(d.params);
            string mname, rname;
            uint32_t serial, refresh, retry, expire, minimum;
            ss>>mname>>rname>>serial>>refresh>>retry>>expire>>minimum;
            vector<unsigned char> mname_vec = parse_domain_name(mname);
            vector<unsigned char> rname_vec = parse_domain_name(rname);

            serial = htonl(serial);
            refresh = htonl(refresh);
            retry = htonl(retry);
            expire = htonl(expire);
            minimum = htonl(minimum);

            int rdata_pos = 0;
            memcpy(&r.rdata[rdata_pos], &mname_vec[0], mname_vec.size());
            rdata_pos += mname_vec.size();
            memcpy(&r.rdata[rdata_pos], &rname_vec[0], rname_vec.size());
            rdata_pos += rname_vec.size();
            memcpy(&r.rdata[rdata_pos], &serial, sizeof(serial));
            rdata_pos += sizeof(serial);
            memcpy(&r.rdata[rdata_pos], &refresh, sizeof(refresh));
            rdata_pos += sizeof(refresh);
            memcpy(&r.rdata[rdata_pos], &retry, sizeof(retry));
            rdata_pos += sizeof(retry);
            memcpy(&r.rdata[rdata_pos], &expire, sizeof(expire));
            rdata_pos += sizeof(expire);
            memcpy(&r.rdata[rdata_pos], &minimum, sizeof(minimum));
            rdata_pos += sizeof(minimum);

            uint16_t rdlength = htons(rdata_pos);
            memcpy(r.rdlength, &rdlength, sizeof(rdlength));
            r.rdata_len = rdata_pos;
            records.push_back(r);
            break;
        }
    }
    return 1;
}

int main(int argc, char* argv[]){
    // parse config
    parse_config(argv[2]);

    // connect to foreign name server
    int connfd = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in servaddr;
    inet_pton(AF_INET, &foreign[0], &servaddr.sin_addr);
	servaddr.sin_family      = AF_INET;
	servaddr.sin_port        = htons(53);
    connect(connfd, (struct sockaddr*) &servaddr, sizeof(servaddr));

    int s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	struct sockaddr_in sin;
    memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_port = htons(strtol(argv[1], NULL, 0));
    bind(s, (struct sockaddr*) &sin, sizeof(sin));

    // the client who connect to this server
    struct sockaddr_in csin;
    memset(&csin, 0, sizeof(csin));
	socklen_t clen = sizeof(csin);

    while(true){
        unsigned char buf[2000], send_buf[2000];
        bzero(buf, 2000);
        bzero(send_buf, 2000);
        int len = recvfrom(s, buf, sizeof(buf), 0, (struct sockaddr*)&csin, &clen);
        memcpy(send_buf, buf, 12); // copy header
        
        swap(buf[0], buf[1]);
        for(int i = 4;i<11;i+=2){
            swap(buf[i], buf[i+1]);
        }
        Header_t* header = (Header_t*) buf;
        
        int pos = 12;
        // fill the question seciton
        Question_t question;
        int next;
        while((next = buf[pos]) != 0){
            pos++;
            string tmp(&buf[pos], &buf[pos+next]);
            question.domain_pieces.push_back(tmp);
            pos+=next;
        }
        question.convert_domain();
        pos++;

        unsigned char* qname = (unsigned char*)malloc((pos-12)*sizeof(unsigned char));
        int qlen = pos-12;
        memcpy(qname, &buf[12], qlen); // record queried domain name
        memcpy(&send_buf[12], &buf[12], pos-12+4); // copy question part into send buffer

        swap(buf[pos], buf[pos+1]);
        swap(buf[pos+2], buf[pos+3]);
        memcpy(&question.Qtype, &buf[pos], 2);
        memcpy(&question.Qclass, &buf[pos+2], 2);
        pos+=4;

        int file_index;
        if((file_index = in_domain(question.domain))!=-1){ // queried domain is handled by dns
            string sub = question.domain;
            sub.erase(sub.begin()+sub.find(local[file_index].first), sub.end());
            if(sub.back()=='.') sub.pop_back();
            if(sub=="") sub="@";

            Header_t* r_header = (Header_t*) send_buf;

            send_buf[2] |= 0x80; // set QR to 1
            send_buf[3] |= 0x80; // set RA to 1
            uint16_t ANCount = 0;
            uint16_t NSCount = 0;
            uint16_t ARCount = 0;

            vector<Record_t> records; // a vec holding records
            switch(question.Qtype){
                case 1: { // A record
                    bool found = false;
                    for(auto &d: local[file_index].second){
                        if(sub==d.sub && d.type=="A"){ // found, return A in answer
                            ANCount ++;
                            Record_t r;
                            memcpy(r.name, qname, qlen);
                            r.name_len = qlen;
                            uint16_t type = htons(1);
                            uint16_t clas = htons(1);
                            uint32_t ttl = htonl(d.ttl);
                            memcpy(r.type, &type, sizeof(type));
                            memcpy(r.clas, &clas, sizeof(clas));
                            memcpy(r.ttl, &ttl, sizeof(ttl));

                            uint16_t rdlength = htons(4);
                            memcpy(r.rdlength, &rdlength, sizeof(rdlength));
                            if(d.params.back()=='\r') d.params.pop_back();
                            inet_pton(AF_INET, d.params.data(), r.rdata);
                            r.rdata_len = 4;
                            records.push_back(r);
                            found = true;
                        }
                    }
                    if(found){ // found, return NS in authority
                        NSCount += put_NS(records, file_index);
                    }
                    else{ // not found, return SOA in authority
                        regex re("^([0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3})\\.([0-9a-zA-Z]{1,61})$");
                        smatch match;
                        if(regex_search(sub, match, re)){
                            string ret = match[1];
                            ANCount = 1;

                            Record_t r;
                            memcpy(r.name, qname, qlen);
                            r.name_len = qlen;
                            uint16_t type = htons(1);
                            uint16_t clas = htons(1);
                            uint32_t ttl = htonl(1);
                            memcpy(r.type, &type, sizeof(type));
                            memcpy(r.clas, &clas, sizeof(clas));
                            memcpy(r.ttl, &ttl, sizeof(ttl));

                            uint16_t rdlength = htons(4);
                            memcpy(r.rdlength, &rdlength, sizeof(rdlength));
                            inet_pton(AF_INET, &ret[0], r.rdata);
                            r.rdata_len = 4;
                            records.push_back(r);
                        }
                        else NSCount += put_SOA(records, file_index);
                    }
                    break;
                }
                case 28: { // AAAA record
                    bool found = false;
                    for(auto &d: local[file_index].second){
                        if(sub==d.sub && d.type=="AAAA"){ // found, return A in answer
                            ANCount ++;
                            Record_t r;
                            memcpy(r.name, qname, qlen);
                            r.name_len = qlen;
                            uint16_t type = htons(28);
                            uint16_t clas = htons(1);
                            uint32_t ttl = htonl(d.ttl);
                            memcpy(r.type, &type, sizeof(type));
                            memcpy(r.clas, &clas, sizeof(clas));
                            memcpy(r.ttl, &ttl, sizeof(ttl));

                            uint16_t rdlength = htons(16);
                            memcpy(r.rdlength, &rdlength, sizeof(rdlength));
                            if(d.params.back()=='\r') d.params.pop_back();
                            inet_pton(AF_INET6, d.params.data(), r.rdata);
                            r.rdata_len = 16;
                            records.push_back(r);
                            found = true;
                        }
                    }
                    if(found){ // found, return NS in authority
                        NSCount += put_NS(records, file_index);
                    }
                    else{ // not found, return SOA in authority
                        NSCount += put_SOA(records, file_index);
                    }
                    break;
                }
                case 2: { // NS record
                    ANCount += put_NS(records, file_index);
                    vector<pair<string, vector<unsigned char>>> ns_domain; 
                    for(auto &d: local[file_index].second){
                        if(d.type=="NS"){
                            // find sub domain name
                            string domain_sub = d.params;
                            domain_sub.erase(domain_sub.begin()+domain_sub.find(local[file_index].first), domain_sub.end());
                            if(domain_sub.back()=='.') domain_sub.pop_back();
                            if(domain_sub=="") domain_sub="@";

                            // parse ns domain name
                            vector<unsigned char> domain_name_vec = parse_domain_name(d.params);
                            ns_domain.push_back(make_pair(domain_sub, domain_name_vec));
                        }
                    }
                    for(auto &m: ns_domain){
                        for(auto &d: local[file_index].second){ // find A record recursively
                            if(m.first==d.sub && d.type=="A"){ // found, return A in answer
                                ARCount ++;
                                Record_t r;
                                memcpy(r.name, &m.second[0], m.second.size());
                                r.name_len = m.second.size();
                                uint16_t type = htons(1);
                                uint16_t clas = htons(1);
                                uint32_t ttl = htonl(d.ttl);
                                memcpy(r.type, &type, sizeof(type));
                                memcpy(r.clas, &clas, sizeof(clas));
                                memcpy(r.ttl, &ttl, sizeof(ttl));

                                uint16_t rdlength = htons(4);
                                memcpy(r.rdlength, &rdlength, sizeof(rdlength));
                                if(d.params.back()=='\r') d.params.pop_back();
                                inet_pton(AF_INET, d.params.data(), r.rdata);
                                r.rdata_len = 4;
                                records.push_back(r);
                            }
                            if(m.first==d.sub && d.type=="AAAA"){ // find AAAA from mx exchange name
                                ARCount ++;
                                Record_t r;
                                memcpy(r.name, &m.second[0], m.second.size());
                                r.name_len = m.second.size();
                                uint16_t type = htons(28);
                                uint16_t clas = htons(1);
                                uint32_t ttl = htonl(d.ttl);
                                memcpy(r.type, &type, sizeof(type));
                                memcpy(r.clas, &clas, sizeof(clas));
                                memcpy(r.ttl, &ttl, sizeof(ttl));

                                uint16_t rdlength = htons(16);
                                memcpy(r.rdlength, &rdlength, sizeof(rdlength));
                                if(d.params.back()=='\r') d.params.pop_back();
                                inet_pton(AF_INET6, d.params.data(), r.rdata);
                                r.rdata_len = 16;
                                records.push_back(r);
                            }
                        }
                    }
                    break;
                }
                case 5: { // CName record
                    bool found = false;
                    for(auto &d: local[file_index].second){ // find A record recursively
                        if(sub==d.sub && d.type=="CNAME"){ // found, return A in answer
                            ANCount ++;
                            Record_t r;
                            memcpy(r.name, qname, qlen);
                            r.name_len = qlen;
                            uint16_t type = htons(5);
                            uint16_t clas = htons(1);
                            uint32_t ttl = htonl(d.ttl);
                            memcpy(r.type, &type, sizeof(type));
                            memcpy(r.clas, &clas, sizeof(clas));
                            memcpy(r.ttl, &ttl, sizeof(ttl));

                            vector<unsigned char> cname = parse_domain_name(d.params);
                            memcpy(r.rdata, &cname[0], cname.size());
                            uint16_t rdlength = htons(cname.size());
                            memcpy(r.rdlength, &rdlength, sizeof(rdlength));
                            r.rdata_len = cname.size();
                            records.push_back(r);
                            found = true;
                        }
                    }
                    if(found) NSCount += put_NS(records, file_index);
                    else NSCount += put_SOA(records, file_index);
                    break;
                }
                case 6: { // SOA record
                    ANCount += put_SOA(records, file_index);
                    NSCount += put_NS(records, file_index);
                    break;
                }
                case 15: { // MX record
                    bool found = false;
                    vector<pair<string, vector<unsigned char>>> mail_domain;
                    for(auto &d: local[file_index].second){ // find A record recursively
                        if(sub==d.sub && d.type=="MX"){ // found, return A in answer
                            ANCount ++;
                            Record_t r;
                            memcpy(r.name, qname, qlen);
                            r.name_len = qlen;
                            uint16_t type = htons(15);
                            uint16_t clas = htons(1);
                            uint32_t ttl = htonl(d.ttl);
                            memcpy(r.type, &type, sizeof(type));
                            memcpy(r.clas, &clas, sizeof(clas));
                            memcpy(r.ttl, &ttl, sizeof(ttl));

                            stringstream ss(d.params);
                            uint16_t preference;
                            string exchange;
                            ss>>preference>>exchange;

                            string domain_sub = exchange;
                            domain_sub.erase(domain_sub.begin()+domain_sub.find(local[file_index].first), domain_sub.end());
                            if(domain_sub.back()=='.') domain_sub.pop_back();
                            if(domain_sub=="") domain_sub="@";

                            // fill in preference
                            uint16_t uint_preference = htons(preference);
                            memcpy(r.rdata, &uint_preference, sizeof(uint16_t));

                            // fill in exchange and save domain name
                            vector<unsigned char> domain_name_vec = parse_domain_name(exchange);
                            mail_domain.push_back(make_pair(domain_sub, domain_name_vec));

                            // fill in rdata
                            memcpy(&r.rdata[2], &domain_name_vec[0], domain_name_vec.size());
                            uint16_t rdlength = htons(domain_name_vec.size()+2);
                            memcpy(r.rdlength, &rdlength, sizeof(rdlength));
                            r.rdata_len = domain_name_vec.size()+2;
                            records.push_back(r);
                            found = true;
                        }
                    }
                    if(found){
                        NSCount += put_NS(records, file_index);
                        for(auto &m: mail_domain){
                            for(auto &d: local[file_index].second){ 
                                if(m.first==d.sub && d.type=="A"){ // find A from mx exchange name
                                    ARCount ++;
                                    Record_t r;
                                    memcpy(r.name, &m.second[0], m.second.size());
                                    r.name_len = m.second.size();
                                    uint16_t type = htons(1);
                                    uint16_t clas = htons(1);
                                    uint32_t ttl = htonl(d.ttl);
                                    memcpy(r.type, &type, sizeof(type));
                                    memcpy(r.clas, &clas, sizeof(clas));
                                    memcpy(r.ttl, &ttl, sizeof(ttl));

                                    uint16_t rdlength = htons(4);
                                    memcpy(r.rdlength, &rdlength, sizeof(rdlength));
                                    inet_pton(AF_INET, d.params.data(), r.rdata);
                                    r.rdata_len = 4;
                                    records.push_back(r);
                                }
                                if(m.first==d.sub && d.type=="AAAA"){ // find AAAA from mx exchange name
                                    ARCount ++;
                                    Record_t r;
                                    memcpy(r.name, &m.second[0], m.second.size());
                                    r.name_len = m.second.size();
                                    uint16_t type = htons(28);
                                    uint16_t clas = htons(1);
                                    uint32_t ttl = htonl(d.ttl);
                                    memcpy(r.type, &type, sizeof(type));
                                    memcpy(r.clas, &clas, sizeof(clas));
                                    memcpy(r.ttl, &ttl, sizeof(ttl));

                                    uint16_t rdlength = htons(16);
                                    memcpy(r.rdlength, &rdlength, sizeof(rdlength));
                                    inet_pton(AF_INET6, d.params.data(), r.rdata);
                                    r.rdata_len = 16;
                                    records.push_back(r);
                                }
                            }
                        }
                        
                    }
                    else NSCount += put_SOA(records, file_index);
                    break;
                }
                case 16: { // TXT record
                    bool found = false;
                    for(auto &d: local[file_index].second){ // find A record recursively
                        if(sub==d.sub && d.type=="TXT"){ // found, return A in answer
                            ANCount ++;
                            Record_t r;
                            memcpy(r.name, qname, qlen);
                            r.name_len = qlen;
                            uint16_t type = htons(16);
                            uint16_t clas = htons(1);
                            uint32_t ttl = htonl(d.ttl);
                            memcpy(r.type, &type, sizeof(type));
                            memcpy(r.clas, &clas, sizeof(clas));
                            memcpy(r.ttl, &ttl, sizeof(ttl));

                            if(d.params.back()=='\r') d.params.pop_back();
                            if(d.params.front()=='"') d.params.erase(0, 1);
                            if(d.params.back()=='"') d.params.pop_back();
                            uint8_t filesize = d.params.size();
                            memcpy(r.rdata, &filesize, 1); // denote string size
                            memcpy(&r.rdata[1], &d.params[0], d.params.size());
                            // unsigned char zero = '\0'; // denote end of the string
                            // memcpy(&r.rdata[filesize+1], &zero, 1);
                            uint16_t rdlength = htons(d.params.size()+1);
                            memcpy(r.rdlength, &rdlength, sizeof(rdlength));
                            r.rdata_len = d.params.size()+1;
                            records.push_back(r);
                            found = true;
                        }
                    }
                    if(found) NSCount += put_NS(records, file_index);
                    else NSCount += put_SOA(records, file_index);
                    break;
                }
            }
            for(auto &i: records){
                memcpy(&send_buf[pos], i.name, i.name_len);
                pos += i.name_len;
                memcpy(&send_buf[pos], i.type, sizeof(i.type));
                pos += sizeof(i.type);
                memcpy(&send_buf[pos], i.clas, sizeof(i.clas));
                pos += sizeof(i.clas);
                memcpy(&send_buf[pos], i.ttl, sizeof(i.ttl));
                pos += sizeof(i.ttl);
                memcpy(&send_buf[pos], i.rdlength, sizeof(i.rdlength));
                pos += sizeof(i.rdlength);
                memcpy(&send_buf[pos], i.rdata, i.rdata_len);
                pos += i.rdata_len;
            }
            uint16_t AN = htons(ANCount);
            uint16_t NS = htons(NSCount);
            uint16_t AR = htons(ARCount);
            memcpy(&send_buf[6], &AN, sizeof(uint16_t)); // set ancount
            memcpy(&send_buf[8], &NS, sizeof(uint16_t)); // set nscount
            memcpy(&send_buf[10], &AR, sizeof(uint16_t)); // set arcount

            sendto(s, send_buf, pos, 0, (sockaddr*) &csin, sizeof(csin));
        }
        else{ // queried domain is not handled by dns ==> send to foreign name server
            send(connfd, send_buf, len, 0);
            bzero(buf, 2000);
            int clen = sizeof(servaddr);
            int len = recvfrom(connfd, buf, 2000, 0, (struct sockaddr*) &servaddr, (socklen_t*)&servaddr);

            sendto(s, buf, len, 0, (sockaddr*) &csin, sizeof(csin));
        }
        free(qname);
    }
}