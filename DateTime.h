// Implementation of a Date Time class
// Diego Enrique Jiménez Urgell A01652617
// 2/12/2020
class DateTime{
private:

    int day;
    string month;
    vector<int> * hour;

    /**
     * Auxiliary method designed to parse an hour given in a string. The numbers are stored in a vector of integers
     * @param hour A string containg the hour, in the format "HH:MM:SS"
     * @return A vector that contains three different integers. The first one for the hour, then minute, and finally seconds.
     */
    vector<int> * parseHour(string hour){
        vector<int> * hourInt = new vector<int>();
        int delim = 0;
        string tmp = "";
        for(int i = 0; i < 2; i++){
            delim = findIndex(hour, 58); // 58 is used because it is the ASCII decimal code of the colon
            tmp = hour.substr(0, delim);
            hourInt -> push_back(stoi(tmp));
            hour = hour.substr(delim + 1);
        }
        hourInt -> push_back(stoi(hour));
        return hourInt;
    }

public:

    /**
     *
     */
    DateTime(){
        this -> hour = NULL;
        this -> day = 0;
        this -> month  = "";
    }

    /**
     *
     * @param m
     * @param d
     * @param h
     */
    DateTime(string m, int d, string h){
        this -> hour = parseHour(h);
        this -> day = d;
        this -> month = m;
    }

    /**
     *
     * @param date2
     * @return
     */
    bool operator == (DateTime * date2){
        if (this -> day != date2 -> day && this -> month.compare(date2 -> month) != 0) return false;
        bool equal = true;
        for(int i = 0; i < 3; i++){
            if ((*(this -> hour))[i] != (*(date2 -> hour))[i]){
                equal = false;
                break;
            }
        }
        return equal;
    }

    /**
     *
     * @param os
     * @param d
     * @return
     */
    friend ostream& operator << (ostream &os, DateTime * d){
        os << d -> month << " " << d -> day << " ";
        os << (*(d -> hour))[0] << ":" << (*(d -> hour))[1]  << ":" << (*(d -> hour))[2];
        return os;
    }


};