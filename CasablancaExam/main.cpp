#include <iostream>
using namespace std;

#include <cpprest/http_client.h>
#include <cpprest/filestream.h>

using namespace utility;                    // Common utilities like string conversions
using namespace web;                        // Common features like URIs.
using namespace web::http;                  // Common HTTP functionality
using namespace web::http::client;          // HTTP client features
using namespace concurrency::streams;       // Asynchronous streams


void GetHttp()
{
	http_client client(U("http://en.cppreference.com/w/"));
	auto resp = client.request(U("GET")).get();

	wcout << U("STATUS : ") << resp.status_code() << endl;
	wcout << "content-type : " << resp.headers().content_type() << endl;
	wcout << resp.extract_string(true).get() << endl;
}

void GetHttpAsync()
{
	http_client client(U("http://en.cppreference.com/w/"));

	client.request(U("GET")).then([](http_response resp) {
		wcout << U("STATUS : ") << resp.status_code() << endl;
		wcout << "content-type : " << resp.headers().content_type() << endl;

		resp.extract_string(true).then([](string_t sBoby) {
			wcout << sBoby << endl;
			}).wait();

		}).wait();

}


json::value k;
string_t PostStart()
{
	http_client client(U("http://localhost:8000/start/tester/0/2"));

	http_request req(methods::POST);

	wcout << U("start --=--") << endl;
	client.request(req).then([=](http_response r) {
		wcout << U("STATUS : ") << r.status_code() << endl;
		wcout << "content-type : " << r.headers().content_type() << endl;

		//{
		//		"time": "11:25:23 AM",
		//		"milliseconds_since_epoch" : 1423999523092,
		//		"date" : "02-15-2015"
		//}

		web::json::value json_par;
		auto pa = json_par.parse(L"{\"title\": \"foo\", \"body\": \"bar\", \"userId\": 1}");


		r.extract_json(true).then([](json::value v) {		
			k = v;
			//wcout << v.to_string() << endl;
			web::json::value ev = v.at(U("elevators"));
			wcout << ev[0].at(U("id")) << endl;
			wcout << ev[0].at(U("floor")) << endl;
			wcout << ev[1].at(U("id")) << endl;
			wcout << ev[1].at(U("floor")) << endl;
			
			wcout << v.at(U("token")) << endl;
			wcout << v.at(U("timestamp")) << endl;
			}).get();

		}).get();

		wcout << k.at(U("token")).as_string() << endl;
		string_t a = k.at(U("token")).as_string();
		wcout << a << endl;

		return k.at(U("token")).as_string();
}

string_t GetTest(string_t token)
{
	http_client client(U("http://localhost:8000/oncalls"));

	wcout << U("oncalls --=--")  << endl;

	http_request req(methods::GET);
	req.headers().add(L"X-Auth-Token", token);

	client.request(req).then([=](http_response r) {
		wcout << U("STATUS : ") << r.status_code() << endl;
		wcout << "content-type : " << r.headers().content_type() << endl;
		
		r.extract_json(true).then([](json::value v) {
			k = v;
			//wcout << v.to_string() << endl;
			web::json::value ev = v.at(U("elevators"));
			wcout << ev[0].at(U("id")) << endl;
			wcout << ev[0].at(U("floor")) << endl;
			wcout << ev[1].at(U("id")) << endl;
			wcout << ev[1].at(U("floor")) << endl;

			wcout << v.at(U("token")) << endl;
			wcout << v.at(U("timestamp")) << endl;
			wcout << "test1 " << v.has_field(U("timestamp")) << endl;	//has_field로 필드가 있는지 확인. 없을 수도 있는데 없으면 에러남. has_boolean 등도 있다
			wcout << "test2 " << v.has_field(U("timestamps")) << endl;//없는 케이스
			}).get();

		}).get();

		wcout << k.at(U("token")).as_string() << endl;
		string_t a = k.at(U("token")).as_string();
		wcout << a << endl;

		return k.at(U("token")).as_string();
}


int main(int argc, char* argv[])
{
	wcout.imbue(locale("kor"));  // windows only 

	string_t token = PostStart();
	GetTest(token);

	//GetHttp();
	//GetHttpAsync();
	//wcout << "asd " << PostStart() << endl;

	return 0;
}