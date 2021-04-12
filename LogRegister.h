// Implementation of the Log Register, which contains a resizing vector graph. It basically adds the connections.
// Diego Enrique Jiménez Urgell A01652617
// 2/12/2020

class LogRegister {

private:
    ResizingVectorGraph graph;

public:

    /**
     * Constructor that initializes the graph with the default constructor, and the source to a NULL pointer (since there
     * are not any nodes yet.
     */
    LogRegister(){
        this -> graph = ResizingVectorGraph();
    }

    /**
     * Adds a new Edge to the Graph. The entry string has the Log entry of the binnacle file. This method parses the source
     * and target IP addresses and creates a connection between the corresponding nodes.
     * Note: The addEdge method manages the case in which a node hasn't been added yet.
     * @param entry A string with the Log entry to be added
     */
    void addConnection(string entry){
        vector<string> data; // To store the information
        int delim = 0;
        string tmp = "";
        for(int i = 0; i < 5; i++){
            delim = findIndex(entry, 32); // Cutting the string everytime a space character is found
            tmp = entry.substr(0, delim);
            data.push_back(tmp);
            entry = entry.substr(delim + 1);
        }

        string month = data[0];
        int day = stoi(data[1]);
        string hour = data[2];

        IP * ip_source = new IP(data[3]);
        IP * ip_target = new IP(data[4]);

        graph.addEdge(ip_source, ip_target, month, day, hour);
    }

    /**
     *
     */
    void printFans(){
        cout << "LIST OF FAN INS AND FAN OUTS PER NODE" << endl;
        this -> graph.printFans();
    }

    /**
     *
     */
    void printAttacks(){
        cout << "LIST OF ATTACKS REGISTERED" << endl;
        this -> graph.printAttacks();
    }

    void printGroupedAttacks(){
        cout << "LIST OF ATTACKS GROUPED PER TARGET" << endl;
        this -> graph.printGroupedAttacks();
    }

    /**
     *
     */
    void printAdj(){
        this -> graph.printAdj();
    }

};