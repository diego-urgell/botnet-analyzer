// Implementation of the IP class, which stores all the digits in an IP address separated in a vector. It manages comparison
// of different IP addresses
// Diego Enrique Jiménez Urgell A01652617
// 2/12/2020

/**
     * An auxiliary method that returns the first index in which a character appears in a string, given its ASCII code.
     * Defined outside of the class since it is a public method used in other classes.
     * @param s The string in which the character is going to be looked for
     * @param ord The ASCII code of the character that is going to be searched.
     * @return The index in which the integer first appears in the string. If the character is not in the string, returns -1
     */
int findIndex(string s, int ord){
    int spaceIndex;
    while(s[spaceIndex] != ord){
        spaceIndex++;
    }
    return spaceIndex;
}

class IP{
private:
    // To store the five fragments of an IP address as separate integers in a vector.
    vector<int> *ip;

    /**
     * An auxiliary method to parse a string with an IP address, and generate a vector with the numbers between each dot or colon.
     * @param ip A string containing the IP address, in the following format: "423.2.230.77:6166"
     * @return A vector containing the separate IP address integers, in the same order in which they appear on the string
     */
    vector<int> * parseIP(string ip){
        vector<int> * ipInt = new vector<int>();
        int delim = 0;
        string tmp = "";
        for (int i = 0; i < 3; i++){
            delim = findIndex(ip, 46); // 46 is used because it is the ASCII decimal code of the dot
            tmp = ip.substr(0, delim);
            ipInt -> push_back(stoi(tmp));
            ip = ip.substr(delim + 1);
        }
        ipInt -> push_back(stoi(ip));
        return ipInt;
    }

public:

    /**
     * Default contructor
     */
    IP():IP(NULL){}

    /**
     * Constructor that initializes the value of the ip vector with the one of a given vector pointer
     * @param ip A pointer to a vector that represents an ip address.
     */
    IP(string ip){
        this -> ip = parseIP(ip);
    }

    /**
     * Method that overrides the "smaller than" operator between an IP object and an IP object pointer. Each fragment of
     * both IP addresses is compared, until two fragments on the same position are found to be different. Therefore, the first
     * different fragment on the IP addresses determines which one is the smaller one.
     * @param ip A pointer to an IP object, to be compared with the element at the right of the operator
     * @return True if the IP from to the left of the operator < is smaller than the one on the right, false otherwise.
     */
    bool operator < (IP *ip) {
        for(int i = 0; i < 4; i++){
            if((*this -> ip)[i] != (*ip -> ip)[i]){
                return this -> ip[i] < ip ->ip[i];
            }
        }
        return false;
    }

    /**
     * Method that overrides the "equal to" operator between an IP object and an IP object pointer.
     * @param ip A pointer to an IP object
     * @return True if the two IP objects contain the same address, false otherwise.
     */
    bool operator == (IP *ip) {
        for(int i = 0; i < 4; i++){
            if((*this -> ip)[i] != (*ip -> ip)[i]){
                return false;
            }
        }
        return true;
    }

    /**
     * Method that overrides the output operator when used with an IP object pointer.
     * @param os The output stream that is going to be printed to (normally cout)
     * @param ip A pointer to an IP object, to be printed
     * @return Returns the output stream with a customized behaviour. It will print the IP address in the correct format
     *          instead of the address of the IP object.
     */
    friend ostream& operator <<(ostream &os, IP *ip){
        for(int i = 0; i < 3; i++){
            os << (*ip ->ip)[i] << ".";
        }
        os << (*ip->ip)[3];
        return os;
    }
};