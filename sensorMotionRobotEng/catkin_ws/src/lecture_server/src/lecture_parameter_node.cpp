//
// Created by jongsoo on 21. 4. 23..
//

#include "ros/ros.h"// ROS 기본 헤더 파일
#include "lecture_server/srvTutorial.h" // srvTutorial 서비스 파일 헤더

#define PLUS 1 // 덧셈
#define MINUS 2 // 빼기
#define MULTIPLICATION 3 // 곱하기
#define DIVISION 4 // 나누기


int g_operator = PLUS;


// 서비스 요청이 있을 경우, 아래의 처리를 수행한다.
// 서비스 요청은 req, 서비스 응답은 res로 설정하였다.
bool calculation(lecture_server::srvTutorial::Request &req,
                 lecture_server::srvTutorial::Response &res) {
// 서비스 요청시 받은 a와 b 값을 파라미터 값에 따라 연산자를 달리한다.
// 계산한 후 서비스 응답 값에 저장한다
    switch (g_operator) {
        case PLUS:
            res.result = req.a + req.b;
            break;
        case MINUS:
            res.result = req.a - req.b;
            break;
        case MULTIPLICATION:
            res.result = req.a * req.b;
            break;
        case DIVISION:
            if (req.b == 0) {
                res.result = 0;
                break;
            } else {
                res.result = req.a / req.b;
                break;
            }
        default:
            res.result = req.a + req.b;
            break;
    }
    // 서비스 요청에 사용된 a, b값의 표시 및 서비스 응답에 해당되는 result 값을 출력한다
    ROS_INFO("request: x=%ld, y=%ld", (long int) req.a, (long int) req.b);
    ROS_INFO("sending back response: [%ld]", (long int) res.result);
    return true;
}

int main(int argc, char **argv)// 노드 메인 함수
{
    ros::init(argc, argv, "parameter_server");// 노드명 초기화
    ros::NodeHandle nh;// 노드 핸들 선언

    nh.setParam("calculation_method", PLUS); // 매개변수 초기설정

    // 서비스 서버 선언,
    // ros_tutorials 패키지의 srvTutorial 서비스 파일을 이용한
    // 서비스 서버 ros_tutorial_service_server를 작성한다.
    // 서비스명은 ros_tutorial_srv이며 서비스 요청이 있을 때,
    // calculation라는 함수를 실행하라는 설정이다.
    ros::ServiceServer lecture_server_server = nh.advertiseService("ros_tutorial_srv", calculation);
    ROS_INFO("ready srv server!");

    ros::Rate r(10); // 10 hz
    while (ros::ok()) {
        nh.getParam("calculation_method", g_operator); // 연산자를 매개변수로부터 받은 값으로 변경한다
        ros::spinOnce(); // 콜백함수 처리루틴
        r.sleep(); // 루틴 반복을 위한 sleep 처리
    }

    return 0;
}

// Parameter 관련하여 설정(setParam) 및 읽기(getParam) 사용법에 주목 할 것!
// [참고] 매개변수로 사용 가능 형태
// • 매개변수는 integers, floats, boolean, string, dictionaries, list 등으로 설정할 수 있다.
// • 간단히 예를 들자면, 1은 integer, 1.0은 floats, “Internet of Things”은 string, true는
// boolean, [1,2,3]은 integers의 list, a: b, c: d는 dictionary이다.

//rosservice call /ros_tutorial_srv 10 5
//result: 15 → 사칙연산의 변수 a, b 입력
//→ 디폴트 사칙연산인 덧셈 결괏값
//$ rosparam set /calculation_method 2
//$ rosservice call /ros_tutorial_srv 10 5
//result: 5