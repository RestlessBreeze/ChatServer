#ifndef CHATSERVICE_H
#define CHATSERVICE_H

#include <muduo/net/TcpConnection.h>
#include <unordered_map>
#include <functional>
#include <mutex>
#include "json.hpp"
#include "usermodel.hpp"
#include "offlinemessagemodel.hpp"
#include "friendmodel.hpp"
#include "groupmodel.hpp"
#include "redis.hpp"

using namespace std;
using namespace muduo::net;
using namespace muduo;
using json = nlohmann::json;

// 表示处理消息的事件回调方法类型
using MsgHandler = std::function<void(const TcpConnectionPtr& conn, json& js, Timestamp time)>;

// 聊天服务器业务类
class ChatService
{
public:
    // 处理登陆业务
    void login(const TcpConnectionPtr& conn, json& js, Timestamp time);

    // 处理注册业务
    void reg(const TcpConnectionPtr& conn, json& js, Timestamp time);

    // 一对一聊天业务
    void oneChat(const TcpConnectionPtr& conn, json& js, Timestamp time);

    // 添加好友业务
    void addFriend(const TcpConnectionPtr& conn, json& js, Timestamp time);

    // 创建群组业务
    void createGroup(const TcpConnectionPtr &conn, json &js, Timestamp time);

    // 加入群组业务
    void addGroup(const TcpConnectionPtr &conn, json &js, Timestamp time);

    // 群组聊天业务
    void groupChat(const TcpConnectionPtr &conn, json &js, Timestamp time);


    void loginout(const TcpConnectionPtr &conn, json &js, Timestamp time);

    // 获取单例对象的接口函数
    static ChatService* instance();
    
    // 获取消息对应的处理器
    MsgHandler getHandler(int msgid);

    // 处理客户端异常退出
    void clientCloseException(const TcpConnectionPtr& conn);

    // 从redis消息队列中获取订阅的消息
    void handleRedisSubscribeMessage(int userid, string msg);

    // 服务器异常业务重置方法
    void reset();

private:
    ChatService();
    
    unordered_map<int, MsgHandler> _msghandlerMap;

    UserModel _userModel;
    OfflineMsgModel _offlineMsgModel;
    FriendModel _friendModel;
    GroupModel _groupModel;

    // 存储在线用户的通信连接
    mutex _connMutex;
    unordered_map<int, TcpConnectionPtr> _userConnMap;

    Redis _redis;
};


#endif