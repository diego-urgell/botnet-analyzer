// Implementation of a Graph data structure that is able to resize. Furthermore, each slot is a vector. (ResizingVectorGraph).
// Diego Enrique Jiménez Urgell A01652617
// 2/12/2020
using namespace std;

//Implementation of a graph using adjacency tables
class Node {
private:
    IP * ip;

public:
    /**
     * Constructor which initializes the node
     * @param ip An IP object pointer
     */
    Node(IP * ip) {
        this->ip = ip;
    }

    /**
     * Method to print the IP when the << operator is called on the node
     * @param os An output stream
     * @param n A node pointer
     * @return The same output stream but with the IP
     */
    friend ostream& operator <<(ostream &os, Node *n){
        os << n -> ip;
        return os;
    }
    friend class ResizingVectorGraph;
};

/**
 * The Edge class is used to represent a connection between two different nodes in the network. An Edge is associated with
 * a date that specifies the moment in which the connection happened. There is also the number of occurences, which is the
 * number of times that the source requested connection with the target in that specific date. This means the Edge is weighted
 * and directed.
 */
class Edge{

private:
    DateTime * dateTime;
    int occurences;

public:

    /**
     * Default constructor of an Edge, sets all atributes to default or NULL.
     */
    Edge(){
        this -> dateTime = NULL;
        this -> occurences = 0;
    }

    /**
     * Constructor that initializes the atributes of Edge given the parameters
     * @param month string
     * @param day int
     * @param hour string "HH:mm:ss"
     */
    Edge(string month, int day, string hour){
        this -> dateTime = new DateTime(month, day, hour);
        this -> occurences = 1;
    }

    /**
     * To add an occurence to the Edge. When a second connection is found between two nodes at the same date, then this
     * method should be used.
     */
    void addOccur(){
        this -> occurences += 1;
    }

    /**
     * Override the default parameter to print the date when an Edge pointer is passed. Mainly for debugging purposes
     * @param os The output stream
     * @param e A pointer to an Edge
     * @return
     */
    friend ostream& operator << (ostream &os, Edge * e){
        os << e -> dateTime;
        return os;
    }

    friend class ResizingVectorGraph;

};

/**
 * This class is an implementation of a graph data structure. It uses an adjacencies table to store the connections between
 * the nodes. However, as there could be several connections in different dates, the date and number of occurences should be
 * stored. To manage this, each slot in the table contains a pointer to a vector of Edge pointers. In this way, more than one
 * edge can exist between two nodes, each one of them in a specific date and with a partiuclar number of occurences (weight),
 * which determine how many connections happened at that specific time. This means the adjacencies table cound be considered to
 * be tridimensional. Furthermore, the number of elements is not fixed; the adjacencies table resizes when there is no more space available.
 */
class ResizingVectorGraph{
private:

    // The adjacencies table. The first two pointer operators are to account for the unknown size, and the third one is to store pointer to vectors.
    vector<Edge*> ***adjacencies;
    int length, capacity; // To determine when to resize
    vector<Node*> nodes; // To store the nodes
    vector<int> fanIn, fanOut; // Fan in and fan outs of each node
    vector<tuple<Node*, Edge*, Node*>*> attacks; // A vector of tuples which stores the attacks in the format <source, edge, target>

    /**
     * Obtains the index of the node with the given value
     * @param value: Value of the node to look up for
     **/
    int getIndex(IP * ip) {
        for (int i = 0; i < nodes.size(); i++) {
            if (*(nodes[i]->ip) == ip)
                return i;
        }
        return -1;  //The value is not a valid one
    }

    /**
     * Auxiliary method to find the coordinates that represent the slot of a particular connection in the adjacencies table.
     * If either of the involved nodes (target or source) is not found in the nodes vector, then it is added.
     * @param source IP pointer: the ip address of the source node
     * @param target IP pointer: the ip address of the target node
     * @return A pointer to an integer vector containing the coordinates of the slot (row, col)
     */
    vector<int> * findSlot(IP * source, IP * target) {
        int row, col; // row -> sources, columns -> targets;
        //Find the index of the source value
        row = getIndex(source);
        if (row == -1){ // If it was not found, add it
            addNode(source);
            row = getIndex(source);
        }
        //Find the index of the target value
        col = getIndex(target);
        if (col == -1) { // If it was not found, add it
            addNode(target);
            col = getIndex(target);
        }
        vector<int> * slot = new vector<int>();
        slot -> push_back(row);
        slot -> push_back(col);

        return slot;
    }

    /**
     * Auxiliary method to double the capacity of the adjacencies table. It copies the past elements in the upper left
     * quadrant and initializes the other ones with pointers to Emtpy edge objects.
     */
    void doubleSize(){
        // The capacity of the adjacencies table is doubled both in width and length
        vector<Edge*> *** tmp = new vector<Edge*>**[capacity*2];
        for (int i=0; i < capacity*2; i++) {
            tmp[i] = new vector<Edge*>*[capacity*2];
        }
        // The existing elements are copied
        for(int i = 0; i < capacity; i++){
            for(int j = 0; j < capacity; j++){
                tmp[i][j] = (adjacencies)[i][j];
            }
        }
        // The lower region (both left and right) is initialized to zero.
        for (int i = capacity; i < capacity*2; i++) {
            for (int j = 0; j < capacity*2; j++) {
                tmp[i][j] = new vector<Edge*>();
            }
        }
        // As we doubled the capacity of a matrix, then we have to fill the upper right region
        for(int i = 0; i < capacity*2; i++) {
            for (int j = capacity; j < capacity * 2; j++) {
                tmp[i][j] = new vector<Edge*>();
            }
        }
        capacity = capacity*2; // The capacity is doubled
        delete [] adjacencies; // The old table is deleted
        adjacencies = tmp;
    }

    /**
     * Auxiliary method to get the fan in count of every node in the graph. Every Edge in the vector in the slot is counted
     */
    void getFanIns(){
        fanIn.clear(); // Clearing the contents of the fan in vector
        for(int k = 0; k < nodes.size(); k++) { // Keeping the target fixed
            int target_count = 0; // Starting to count the number of connections
            for (int i = 0; i < nodes.size(); i++) {
                for (int j = 0; j < (adjacencies)[i][k] -> size(); j++) {
                    target_count += (*(adjacencies[i][k]))[j] -> occurences;
                }
            }
            fanIn.push_back(target_count);
        }
    }

    /**
     * Auxiliary method to get the fan out count of every node in the graph. Every Edge in the vector in the slot is counted
     */
    void getFanOuts(){
        fanOut.clear(); // Clearing the contents of the fan out vector
        for(int k = 0; k < nodes.size(); k++){ // Keeping the source fixed
            int source_count = 0; // Starting to count the number of connections
            for(int i = 0; i < nodes.size(); i++){
                for(int j = 0; j < (adjacencies)[k][i] -> size(); j++){
                    source_count += (*(adjacencies[k][i]))[j] -> occurences;
                }
            }
            fanOut.push_back(source_count);
        }
    }

    // row -> source, col -> target, [row][col]
    /**
     * Auxiliary method to get the attacks carried out in the network. Is considered to be an attack if more than three
     * connections were established from one source to a target in the same instant. For each attack a tuple is created in
     * the format <source, edge, target>. Pointers to the tuples are storedd in the attacks vector
     */
    void getAttackers(){
        attacks.clear();
        for(int i = 0; i < nodes.size(); i++){ // Keeping the source fixed
            for(int j = 0; j < nodes.size(); j++){ // Moving to all the targets of the same source
                for(int k = 0; k < (adjacencies)[i][j] -> size(); k++){
                    if ((*(adjacencies[i][j]))[k] -> occurences >= 3){ // If there were more than tree connections
                        tuple<Node*, Edge*, Node*> * attack = // Create the tuple and store it.
                                new tuple<Node*, Edge*, Node*>(nodes[i], (*(adjacencies[i][j]))[k], nodes[j]);
                        attacks.push_back(attack);
                    }
                }
            }
        }
    }

public:

    /**
     * Default constructor. Initializes the graph to 10 elements
     */
    ResizingVectorGraph():ResizingVectorGraph(10){}


    /**
     * Constructor that initializes the graph to a particular initial size.
     * @param initSize int
     */
    ResizingVectorGraph(int initSize){
        this -> adjacencies = new vector<Edge*>**[initSize]; // Creating the "first dimension"
        for(int i = 0; i < initSize; i++){ // For each linear slot
            this -> adjacencies[i] = new vector<Edge*>*[initSize]; // Create another linear slot, the "second dimension".
            for(int j = 0; j < initSize; j++){
                adjacencies[i][j] = new vector<Edge*>();
            }
        }
        this -> length = 0;
        this -> capacity = initSize;
    }

    /**
     * Adds a new node with the given value to the graph. If there is no space in the adjacencies table, then a new matrix is
     * created on the heap, with twice the capacity as the old one, by calling a method. Then the new node is added
     * @param ip a pointer to the IP of the node.
     **/
    void addNode(IP * ip) {
        length++; // Increase the length (number of indexes occupied)
        if(length >= capacity) this -> doubleSize();
        Node *n = new Node(ip); // Adding the new node
        nodes.push_back(n);
    }

    /**
     * Method to add a new Edge to the network. First, the coordinates of the row and column of the adjacencies table are
     * obtained given the source and target IPs. Then, if the connection time matches the time in an Edge already present
     * in the slot, only an occurence is added. If it is a new time, then an Edge is added.
     * @param source: IP pointer
     * @param target: IP pointer
     * @param month: string
     * @param day: day
     * @param hour: string
     */
    void addEdge(IP * source, IP * target, string month, int day, string hour){
        vector<int> * coordinates = this -> findSlot(source, target); // Find the coordinates
        int row = (*coordinates)[0];
        int col = (*coordinates)[1];
        DateTime * dt = new DateTime(month, day, hour);
        bool found = false;
        vector<Edge*> * currSlot = (this -> adjacencies)[row][col]; // The slot in the table
        for(int i = 0; i < currSlot -> size(); i++){
            if ((*(*currSlot)[i] -> dateTime) == dt){ // Compare if the time in a past edge matches the time in the current connection
                found = true;
                (*currSlot)[i] -> addOccur(); // If that is the case, then just add an occurence and toggle the flag.
            }
        }
        if(!found){ // If the same date was not found
            Edge * newEdge = new Edge(month, day, hour);
            currSlot -> push_back(newEdge); // Create a new Edge and store it.
        }
        delete dt;
    }

    /**
     * Method to print the fan in and fan out of each node.
     */
    void printFans(){
        this -> getFanIns();
        this -> getFanOuts();
        for(int i = 0; i < fanIn.size(); i++){
            cout << "IP: " << nodes[i] << "    Fan In: " << fanIn[i] << "    Fan Out: " << fanOut[i] << endl;
        }
    }


    /**
     * Method to print the attacks registered, without grouping.
     */
    void printAttacks(){
        this -> getAttackers();
        for(int i = 0; i < attacks.size(); i++){
            cout << "Attacker: " << get<0>(*(attacks[i])) << "   Date Time: " << get<1>(*(attacks[i])) -> dateTime
            << "   Number of attacks: " << get<1>(*(attacks[i])) -> occurences << "   Target: " << get<2>(*(attacks[i])) << endl;
        }
    }

    /**
     * Method to print the attacks registered in the network, but grouping the attacks by target. This means that if
     * various sources attacked a same target, they will appear in a group. STL multimap structure was used, since the key
     * was the target and several entries could have the same key (several attacks could have the same target).
     */
    void printGroupedAttacks(){
        this -> getAttackers(); // First get the atttackers
        multimap<Node*, tuple<Node*, Edge*, Node*>> grouped; // Create a multimap with the target as key and the attack tuple as value
        for(int i = 0; i < attacks.size(); i++){ // For each registered attack, add it to the multimap
            grouped.insert(pair<Node*, tuple<Node*, Edge*, Node*>> (get<2>(*(attacks[i])), *(attacks[i])));
        }
        auto itr1= grouped.begin(); // Main iterator
        while(itr1 != grouped.end()){ // While it is inside the multimap entries
            cout << "\nTARGET: " << itr1 -> first << endl;
            int totalNum = 0; // Total number of attacks to a partiuclar target
            auto itr2 = grouped.equal_range(itr1 -> first); // Find the range of elements with the same key (attacks with the same target)
            for(auto itr3 = itr2.first; itr3 != itr2.second; itr3++){ // For each attack with the same target
                tuple<Node*, Edge*, Node*> * attack = &itr3 -> second;
                totalNum += get<1>(*(attack)) -> occurences; // Add the weight to the number of attacks per target
                cout << "------- Date Time: " << get<1>(*(attack)) -> dateTime << "   Attacker: " << get<0>(*(attack))
                     << "   Number of attacks: " << get<1>(*(attack)) -> occurences << endl; // Print the information
            }
            cout << "Total number of attacks: " << totalNum << endl; // Print the total number of attacks
            // Change the main iterator to the end of the equal range. This means the first occurence of a different key (target).
            itr1 = itr2.second;
        }
    }

    /**
     * Method to print the adjacencies table. Used for debugging purposes
     */
    void printAdj(){
        for(int i = 0; i < nodes.size(); i++){
            for(int j = 0; j < nodes.size(); j++){
                if((adjacencies[i][j]) -> size() > 0 ) {
                    cout << (*(adjacencies[i][j]))[0] ->occurences << "  "; // [source][target]
                } else{
                    cout << "0 ";
                }
            }
            cout << endl;
        }
    }

};