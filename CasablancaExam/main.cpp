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
	http_client client(U("http://localhost:8000"));

	http_request req(methods::POST);
	req.set_request_uri(L"/start/tester/0/2");

	wcout << U("start --=--") << endl;
	client.request(req).then([&](http_response r) {
		wcout << U("STATUS : ") << r.status_code() << endl;
		wcout << "content-type : " << r.headers().content_type() << endl;

		//{
		//		"time": "11:25:23 AM",
		//		"milliseconds_since_epoch" : 1423999523092,
		//		"date" : "02-15-2015"
		//}

		web::json::value json_par;
		auto pa = json_par.parse(L"{\"title\": \"foo\", \"body\": \"bar\", \"userId\": 1}");


		r.extract_json(true).then([&](json::value v) {		
			k = v;
			
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

json::value GetTest(string_t token)
{
	http_client client(U("http://localhost:8000"));

	wcout << U("oncalls --=--")  << endl;

	http_request req(methods::GET);
	req.set_request_uri(L"/oncalls");
	req.headers().add(L"X-Auth-Token", token);

	json::value GT;

	client.request(req).then([&](http_response r) {
		wcout << U("STATUS : ") << r.status_code() << endl;
		wcout << "content-type : " << r.headers().content_type() << endl;
		
		r.extract_json(true).then([&](json::value v) {
			GT = v;
			wcout << v.serialize() << endl;//json 姥繕端研 壕伸 莫殿稽 窒径.
			//wcout << v.to_string() << endl;
			json::value ev = v.at(U("elevators"));
			wcout << "size " << ev.as_array().size() << endl;
			wcout << ev[0].at(U("id")) << endl;
			wcout << ev[0].at(U("floor")) << endl;
			wcout << ev[1].at(U("id")) << endl;
			wcout << ev[1].at(U("floor")) << endl;

			wcout << v.at(U("token")) << endl;
			wcout << v.at(U("timestamp")) << endl;
			wcout << "test1 " << v.has_field(U("timestamp")) << endl;	//has_field稽 琶球亜 赤澗走 溌昔. 蒸聖 呪亀 赤澗汽 蒸生檎 拭君害. has_boolean 去亀 赤陥
			wcout << "test2 " << v.has_field(U("timestamps")) << endl;//蒸澗 追戚什
			
			}).get();

		}).get();


		wcout << "sd" <<  GT.at(U("token")) << endl;

		return GT;
}

void te() {
	web::json::value json_return;
	web::json::value json_v;

	json_v[L"title"] = web::json::value::string(L"foo");
	json_v[L"body"] = web::json::value::string(L"bar");
	json_v[L"userId"] = web::json::value::number(1);
	web::http::client::http_client client(L"https://jsonplaceholder.typicode.com/posts");
	client.request(web::http::methods::POST, U("/"), json_v)
		.then([](const web::http::http_response& response) {
		return response.extract_json();
			})
		.then([&json_return](const pplx::task<web::json::value>& task) {
				try {
					json_return = task.get();
				}
				catch (const web::http::http_exception& e) {
					std::cout << "error " << e.what() << std::endl;
				}
			})
				.wait();

			std::wcout << json_return.serialize() << std::endl;
}

//void tes() {
//	return pplx::create_task([]
//		{
//			json::value postData;
//			postData[L"name"] = json::value::string(L"Joe Smith");
//			postData[L"sport"] = json::value::string(L"Baseball");
//
//			http_client client(L"http://localhost:5540/api/values");
//			return client.request(methods::POST, L"",
//				postData.to_string().c_str(), L"application/json");
//		}).then([](http_response response)
//			{
//				if (response.status_code() == status_codes::OK)
//				{
//					auto body = response.extract_string();
//					std::wcout << L"Added new Id: " << body.get().c_str() << std::endl;
//
//					return std::stoi(body.get().c_str());
//				}
//
//				return 0;
//			});
//}

string_t PostTest(string_t token)
{
	wcout << U("action --=--") << endl;
	http_client client(U("http://localhost:8000/action"));
	
	http_request req(methods::POST);
	//req.set_request_uri(L"/actions");
	req.headers().add(L"X-Auth-Token", token);

	web::json::value json_param;
	string_t input = L"{\"commands\":[{\"elevator_id\":0,\"command\":\"UP\"},{\"elevator_id\":1,\"command\":\"STOP\"}]}";
	auto pa = json_param.parse(input);
	wcout << pa[L"commands"][0][L"elevator_id"] << endl;
	wcout << pa[L"commands"][0][L"command"] << endl;
	wcout << pa[L"commands"][1][L"elevator_id"] << endl;
	wcout << pa[L"commands"][1][L"command"] << endl;
	wcout << pa.serialize() << endl;
	wcout << token << endl;

	req.set_body(pa);
	wcout << "content-type : " << req.headers()[L"X-Auth-Token"] << endl;
	wcout << "content-len : " << req.headers().content_length() << endl;

	client.request(req).then([&](http_response r) {
		wcout << U("STATUS : ") << r.status_code() << endl;
		wcout << "content-type : " << r.headers().content_type() << endl;

		//{
		//		"time": "11:25:23 AM",
		//		"milliseconds_since_epoch" : 1423999523092,
		//		"date" : "02-15-2015"
		//}


		r.extract_json(true).then([&](json::value v) {
			k = v;

			wcout << v.serialize() << endl;//json 姥繕端研 壕伸 莫殿稽 窒径.
			/*web::json::value ev = v.at(U("elevators"));
			wcout << "size " << ev.as_array().size() << endl;

			wcout << ev[0].at(U("id")) << endl;
			wcout << ev[0].at(U("floor")) << endl;
			wcout << ev[1].at(U("id")) << endl;
			wcout << ev[1].at(U("floor")) << endl;

			wcout << v.at(U("token")) << endl;
			wcout << v.at(U("timestamp")) << endl;*/
			}).get();

		}).get();

		return L"";

		wcout << k.at(U("token")).as_string() << endl;
		string_t a = k.at(U("token")).as_string();
		wcout << a << endl;

		return k.at(U("token")).as_string();
}

//json 神崎詮闘 植室什 号狛 3亜走
//gt.at(U("token"))
//gt[U("token")]
//gt[L"token"]
//是 号狛生稽 羨悦馬檎 据 莫縦生稽 軒渡鞠澗 牛. 暗奄辞 as_string, as_integer 去聖 硲窒 亜管. 舘 陥献 莫縦析 獣 拭君.
//据 莫縦戚檎 string 析凶 "けいしけいし" 戚訓縦生稽 窒径.

//json array虞檎 gt[0], gt[2] 人 旭戚 羨悦 亜管. as_array().size()稽 滴奄 羨悦 亜管

int main(int argc, char* argv[])
{
	wcout.imbue(locale("kor"));  // windows only 

	string_t token = PostStart();
	auto gt = GetTest(token);

	wcout << gt.at(U("token")) << endl;
	wcout << gt.at(U("timestamp")) << endl;
	wcout << gt[U("token")] << endl;
	wcout << gt[L"timestamp"] << endl;
	//gt = GetTest(token);


	te();
	PostTest(token);

	//GetHttp();
	//GetHttpAsync();
	//wcout << "asd " << PostStart() << endl;

	return 0;
}