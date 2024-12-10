# Social Network Simulation System

This project simulates a basic social network with functionalities such as friend requests, messaging, content posting, interaction tracking, and recommendations for friends and content. It utilizes concepts like multi-threading, caching (LRU Cache), and graph traversal (DFS) for its features.

---

## Features

1. **User Management**
   - Add users to the system.
   - Manage friendships (add and delete).

2. **Content Management**
   - Post content.
   - Interact with content (like, comment, etc.).

3. **Messaging System**
   - Send messages between users.
   - Retrieve message queues.

4. **Recommendations**
   - Friend recommendations based on graph traversal (DFS).
   - Content recommendations with an LRU cache mechanism.

5. **Server System**
   - Distributes load using consistent hashing.
   - Manages requests for various functionalities.

---

## Project Structure

### Classes and Their Responsibilities

1. **`User`**
   - Represents a user in the system.
   - Manages friend requests, content creation, and message sending.

2. **`Content`**
   - Represents a piece of content created by a user.
   - Tracks interactions.

3. **`Request`**
   - Represents a request sent to the server (e.g., adding friends, posting content).

4. **`LRUCache`**
   - Caches recently accessed content for recommendations.

5. **`Database`**
   - Stores all users, content, and interactions in memory.
   - Manages user-to-content and user-to-user relationships.

6. **`Server`**
   - Processes requests for various operations.
   - Uses caching and traversal algorithms for recommendations.

7. **`System`**
   - Manages the servers.
   - Implements consistent hashing for load balancing.

---

## Installation and Setup

1. Clone the repository:
   ```bash
   git clone <repository-url>
   cd <repository-directory>

2. Compile the code:
   ```bash
   g++ -std=c++17 main.cpp -o system_simulation
3. Run the program:
   ```bash
   ./system_simulation


## Usage
Adding Users

Create User objects and add them to the system using Database::addUser().
Creating Requests

Use methods in User to create requests for:
Adding friends: CreateRequest(User u, bool isDel)
Posting content: CreateRequest(Content c)
Interacting with content: CreateRequest(Content c, int interactionType)
Sending messages: CreateRequest(User u, string msg)
Getting recommendations: CreateRequest()
Processing Requests

Add requests to the server queue using Server::AddRequest(Request r).
Call Server::ProcessRequests() to process the requests.
Content Recommendations

Use the Server's content recommendation feature to suggest content based on user interactions.
Friend Recommendations

Get friend suggestions based on the graph traversal algorithm implemented in the server.
