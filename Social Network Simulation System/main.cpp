#include <algorithm>
#include <atomic>
#include <chrono>
#include <iostream>
#include <list>
#include <map>
#include <mutex>
#include <queue>
#include <thread>
#include <unordered_map>
#include <unordered_set>
using namespace std;

class Server;
class User;
class Content {
public:
  int contentID;
  User* u;
  string S;
  Content(int contentID, User* u, string S){
    this->contentID = contentID;
    this->u = u;
    this->S = S;
  }
};
class Request {
public:
  int type;
  int reqID;
  vector<Content> ContentArr;
  vector<User*> UserArr;
  string message;
  int interactionType;
  static int lastReq;
  Request(int type, const std::vector<Content> &ContentArr = {},
          const std::vector<User*> &UserArr = {},
          const std::string &message = "", int interactionType = 0)
      : type(type), ContentArr(ContentArr), UserArr(UserArr), message(message),
        interactionType(interactionType) {
    this->reqID = lastReq;
    lastReq++;
  }
};
int Request::lastReq = 0;

class User {
public:
  int UserID;
  queue<pair<pair<int, int>, string>> msgQueue;
  static int lastID;
  // function overloading
  User() {
    UserID = lastID + 1; // Assignment after the constructor is called
    lastID++;
    // Start the message retrieval thread when a User object is created
    // messageThread = std::thread(&User::messageLoop, this);
  }

  // ~User() {
  //   // Make sure to join the thread before destruction
  //   if (messageThread.joinable()) {
  //     stopRequested = true;
  //     messageThread.join();
  //   }
  // }
  Request CreateRequest(User u, bool isDel) {
    // Addfriend& DeleteFriend
    vector<User*> temp;
    cout << this->UserID << " is creating a request to make " << u.UserID << " his friend" << endl;
    temp.push_back(this);
    temp.push_back(&u);
    int id = (isDel) ? 102 : 101;
    Request newReq(id, {}, temp);
    return newReq;
  }
  Request CreateRequest(User u, string msg) {
    // send message
    vector<User*> temp;
    temp.push_back(this);
    temp.push_back(&u);
    int id = 102;
    Request newReq(id, {}, temp, msg);
    return newReq;
  }
  Request CreateRequest(Content C) {
    // POst Content
    vector<Content> temp;
    temp.push_back(C);
    int id = 103;
    Request newReq(id, temp);
    return newReq;
  }
  Request CreateRequest(Content C, int interaction) {
    // liked a content
    vector<User*> temp1;
    temp1.push_back(this);
    vector<Content> temp;
    temp.push_back(C);
    int id = 104;
    Request newReq(id, temp, temp1, "", interaction);
    return newReq;
  }
  Request CreateRequest() {
    // Recommend User
    vector<User*> temp;
    int id = 105;
    Request newReq(id, {}, temp);
    return newReq;
  }
  void getMessages() {
    while (!msgQueue.empty()) {
      cout << msgQueue.front().second << endl;
      msgQueue.pop();
    }
  }

// private:
//   thread messageThread;
//   bool stopRequested = false;

//   void messageLoop() {
//     while (!stopRequested) {
//       getMessages();
//       std::this_thread::sleep_for(
//           std::chrono::seconds(2)); // Adjust the interval as needed
//     }
//   }
};
int User::lastID = 0;



class LRUCache {
private:
  int capacity;
  list<Content> cacheList;
  unordered_map<int, list<Content>::iterator> cacheMap;

  // List: This will maintain the order of elements, with the most recently used
  // element at the front. Map: This will map content IDs to their positions
  // (iterators) in the list.

public:
  LRUCache(int capacity) : capacity(capacity) {}

  string get(int contentID) {
    if (cacheMap.find(contentID) == cacheMap.end()) {
      return "";
    }
    // Move the accessed element to the front of the list
    cacheList.splice(cacheList.begin(), cacheList, cacheMap[contentID]);
    return cacheMap[contentID]->S;
  }

  void put(Content &content) {
    if (cacheMap.find(content.contentID) != cacheMap.end()) {
      // Update existing content and move it to the front
      cacheList.splice(cacheList.begin(), cacheList,
                       cacheMap[content.contentID]);
      cacheMap[content.contentID] = cacheList.begin();
      return;
    }

    if (cacheList.size() == capacity) {
      // Remove the least recently used content
      int lastContentID = cacheList.back().contentID;
      cacheList.pop_back();
      cacheMap.erase(lastContentID);
    }

    // Insert the new content at the front
    cacheList.push_front(content);
    cacheMap[content.contentID] = cacheList.begin();
  }
};

class Database {
public:
  static unordered_map<int, vector<pair<int, int>>>
      userContentMap; // {userID, {interactionType, contentID}}
  static unordered_map<int, vector<pair<int, int>>>
      contentUserMap; // {contentID, {userID, interactionType}}
  static unordered_map<int, vector<int>> adjacencyList; // {userID, [friendIDs]}
  static vector<Content> contents;                      // List of all contents
  static vector<User> users;                            // List of all users

  static void addInteraction(int userId, Content &content,
                             int interactionType) {
    userContentMap[userId].push_back({interactionType, content.contentID});
    contentUserMap[content.contentID].push_back({userId, interactionType});
  }

  static void addFriend(int user1, int user2) {
    cout<<"!"<<user1<<"@"<<user2<<endl;
    adjacencyList[user1].push_back(user2);
    adjacencyList[user2].push_back(user1);
  }

  static void deleteFriend(int user1, int user2) {
    // Remove user2 from user1's friend list
    auto &friends1 = adjacencyList[user1];
    for (auto it = friends1.begin(); it != friends1.end(); ++it) {
      if (*it == user2) {
        std::swap(*it, friends1.back());
        friends1.pop_back();
        break;
      }
    }

    // Remove user1 from user2's friend list
    auto &friends2 = adjacencyList[user2];
    for (auto it = friends2.begin(); it != friends2.end(); ++it) {
      if (*it == user1) {
        std::swap(*it, friends2.back());
        friends2.pop_back();
        break;
      }
    }
  }

  static void addContent(Content &content) { contents.push_back(content); }

  static void addUser(User user) { users.push_back(user); }
};
unordered_map<int, vector<pair<int, int>>>Database::
    userContentMap = {}; // {userID, {interactionType, contentID}}
unordered_map<int, vector<pair<int, int>>>Database::
    contentUserMap={}; // {contentID, {userID, interactionType}}
unordered_map<int, vector<int>> Database :: adjacencyList = {};// {userID, [friendIDs]}
vector<Content> Database :: contents;                      // List of all contents
vector<User> Database :: users;                            // List of all users

void dfs(int depth, int userId, unordered_set<int> &visited,
         vector<User> &ans) {
  if (depth == 0)
    return;

  visited.insert(userId);

  for (int friendId : Database::adjacencyList[userId]) {
    if (visited.find(friendId) == visited.end()) {
      ans.push_back(Database::users[friendId]);
      dfs(depth - 1, friendId, visited, ans);
    }
  }
}

class Server {
public:
  int serverID;
  int activeConnections;
  std::queue<Request> requestQueue;
  LRUCache cache;
  // std::mutex mtx;

  Server(int id, size_t cacheCapacity)
      : serverID(id), activeConnections(0), cache(cacheCapacity) {}

  void RequestHandler(Request r) {
    if (r.type == 101) {
      // ADD_FRIEND
      Database ::addFriend(r.UserArr[0]->UserID, r.UserArr[1]->UserID);
    } else if (r.type == 102) {
      // DELETE_FRIEND
      Database ::deleteFriend(r.UserArr[0]->UserID, r.UserArr[1]->UserID);
    } else if (r.type >= 1 && r.type <= 3) {
      // add interaction
      Database ::addInteraction(r.UserArr[0]->UserID, r.ContentArr[0],
                                r.interactionType);
    } else if (r.type == 201) {
      // SEND_MESSAGE
      User &sender = (*r.UserArr[0]);
      User &receiver = (*r.UserArr[1]);
      string messagetext = r.message;
      sender.msgQueue.push({{sender.UserID, receiver.UserID}, messagetext});
    } else if (r.type == 301) {
      // CONTENT_POST
      Database::addContent(r.ContentArr[0]);
    } else if (r.type == 401) {
      // CONTENT_RECOMMENDATION
      unordered_set<int> visited;
      int curr_user = r.UserArr[0]->UserID;
      vector<User> ans;
      dfs(3, curr_user, visited, ans);
      vector<string> ansi;
      for (auto &user : ans) {
        for (auto &c : Database::userContentMap[user.UserID]) {
          if (cache.get(c.second) == "") {
            for (auto &content : Database::contents) {
              if (content.contentID == c.second) {
                cache.put(content);
                break;
              }
            }
          } else {
            ansi.push_back(cache.get(c.second));
          }
        }
      }
      for (auto c : ansi) {
        cout << c << " ";
      }
      cout << endl;
    } else if (r.type == 501) {
      // FRIEND_RECOMMENDATION
      unordered_set<int> visited;
      int curr_user = r.UserArr[0]->UserID;
      vector<User> ans;
      dfs(3, curr_user, visited, ans);
      for (auto &user : ans) {
        cout << user.UserID << " ";
      }
      cout << endl;
    } else {
      cout << "Invalid Request" << endl;
    }
  }

  void ProcessRequests() {
    // while (true) {
    //   std::this_thread::sleep_for(std::chrono::seconds(5));
    //   mtx.lock();
      while (!requestQueue.empty()) {
        Request r = requestQueue.front();
        requestQueue.pop();
        RequestHandler(r);
      }
      // mtx.unlock();
    // }
  }

  int GetServerID() const { return serverID; }

  void AddRequest(Request r) {
    // std::lock_guard<std::mutex> lock(mtx);
    requestQueue.push(r);
  }
};

class System {
private:
  vector<Server> servers;
  queue<Request> requestQueue;
  atomic<int> nextServerId;
  map<size_t, int> hashRing; // Map from hash value to server ID
  size_t hashFunction(int key) const { return hash<int>{}(key); }

public:
  System() : nextServerId(0) {}

  void LoadBalancingandSend(Request r) {
    if (hashRing.empty()) {
      cout << "No servers available to handle the request." << endl;
      return;
    }

    size_t hash =
        hashFunction(r.reqID); // Step 1: Compute hash of the request ID
    auto it = hashRing.lower_bound(
        hash); // Step 2: Find the closest server in the hash ring

    if (it == hashRing.end()) {
      it = hashRing.begin(); // Wrap around to the beginning if the hash is
                             // larger than all existing hashes
    }

    int serverId =
        it->second; // Step 3: Get the server ID corresponding to the hash
    auto serverIt = find_if(
        servers.begin(), servers.end(), [serverId](const Server &server) {
          return server.serverID ==
                 serverId; // Find the server object by serverID
        });

    if (serverIt != servers.end()) {
      serverIt->requestQueue.push(
          r); // Step 4: Add the request to the server's queue
      serverIt->RequestHandler(r); // Handle the request
      cout << "Request sent to server with ID " << serverIt->serverID << endl;
    } else {
      cout << "System unavailable" << endl;
    }
  }

  void AddServer(size_t cacheCapacity) {
    int id = nextServerId++; // Generate a new server ID
    servers.emplace_back(
        id, cacheCapacity); // Create a new server with the given cache capacity

    // Add server to the hash ring
    for (int i = 0; i < 3;
         ++i) { // Adding virtual nodes for better distribution
      size_t hash =
          hashFunction(id + i); // Compute a hash for each virtual node
      hashRing[hash] = id; // Map the hash to the server ID in the hash ring
    }

    cout << "Server with ID " << id << " added." << endl;
  }

  void RemoveServer(int serverId) {
    servers.erase(
        remove_if(servers.begin(), servers.end(),
                  [serverId](const Server &server) {
                    return server.serverID ==
                           serverId; // Remove the server from the list
                  }),
        servers.end());

    // Remove server from the hash ring
    for (int i = 0; i < 3; ++i) { // Removing virtual nodes
      size_t hash =
          hashFunction(serverId + i); // Compute hash for each virtual node
      hashRing.erase(hash);           // Remove the hash from the hash ring
    }
  }

  void AddRequestToQueue(Request r) { requestQueue.push(r); }

  void ProcessQueue() {
    while (!requestQueue.empty()) {
      Request r = requestQueue.front();
      requestQueue.pop();
      LoadBalancingandSend(r);
    }
  }
};

int main() {
  System system;
  system.AddServer(10);
  system.AddServer(50);

  User user1;
  User user2;
  User user3;
  cout << user1.UserID << " " << user2.UserID << " " << user3.UserID << endl;
  Request req=user1.CreateRequest(user2,false);//Addfriend
  Request req1=user1.CreateRequest(user3,false);//Addfriend
  system.AddRequestToQueue(req);
  system.AddRequestToQueue(req1);
  system.ProcessQueue();
  
  for(auto it:Database::adjacencyList){
    cout << it.first << ": ";
    for(auto it2:it.second){
      cout << it2 << " ";
    }cout << endl;
  }
  Request req3=user1.CreateRequest(user3,true);//delete
  system.AddRequestToQueue(req3);
  system.ProcessQueue();
  
  for(auto it:Database::adjacencyList){
    cout << it.first << ": ";
    for(auto it2:it.second){
      cout << it2 << " ";
    }cout << endl;
  }
  return 0;
}