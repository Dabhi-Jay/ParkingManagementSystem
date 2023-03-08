// PARKING MANAGEMENT SYSTEM

// INPUT: CHECKIN GJ03DF-2708 08:15am NR
// OUTPUT:A-2
// INPUT: CHECKIN GJT-3293 09:00am R
// OUTPUT:A-1
// INPUT:CHECKOUT GJ03DF-2708 11:45am
// OUTPUT:80
// INPUT:CHECKOUT GJT-3293 12:00pm
// OUTPUR:80
// INPUT:CHECKIN GJ09DF-9687 12:15pm NR
// OUTPUT:A-2
// INPUT:CHECKOUT GJ09DF-9687 12:45pm
// OUTPUT:50
// INPUT:GENERATE REPORT
// OUTPUT:

// PARKING SLOT, CAR NO, CHECK IN TIME, CHECK OUT TIME, CHARGES, CATEGORY
// A-1, GJT-3293, 09:00am, 12:00pm 80, R
// A-2, GJ03DF-2708, 08:15am, 11:45am, 80, NR
// A-2, GJ09DF-9687, 12:15pm, 12:45pm, 50, NR
#include<bits/stdc++.h>
using namespace std;
vector<string> splitByDelimeter(const string &s,char ch)
{
    vector<string> ans;
    string temp;
    for (int i = 0; i < s.length(); i++)
    {
        if (s[i] != ch)
        {
            temp += s[i];
        }
        else
        {
            ans.push_back(temp);
            temp = "";
        }
    }
    ans.push_back(temp);
    return ans;
}
class Vehicles
{
    public:
            vector<string>slot;
            vector<string>numberPlate;
            vector<string>timeIn;
            vector<string>timeOut;
            vector<int>charge;
            vector<string>category;
};
void print(Vehicles v)
{
    for(int i=0;i<v.numberPlate.size();i++)
    {
        cout<<v.slot[i]<<" "<<v.numberPlate[i]<<" "<<v.timeIn[i]<<" "<<v.timeOut[i]<<" "<<v.charge[i]<<" "<<v.category[i]<<"\n";
    }
    cout<<"\n";
}
class System
{
    private:
            int calculateMinutes(Vehicles &,int &);
    public:
            int n=5,x=10;
            vector<string> reservedSlots = {"A-1", "A-10", "B-2", "B-5", "C-1", "C-8", "D-2", "D-4", "E-5", "E-10"};
            int numberOfReserveSlots=reservedSlots.size();
            vector<vector<int>> matrix;
            bool checkin(string &,Vehicles &);
            void checkout(Vehicles &,string ,string );
            void generateReport(Vehicles &,vector<string>&);
};
//Calculates total minutes between timeOfEntering and timeOfLeaving for vehicle
int System :: calculateMinutes(Vehicles &v,int &i)
{
    int minutes=0;
    if (v.timeOut[i].substr(5) != v.timeIn[i].substr(5))
    {
        minutes = 60 - stoi(v.timeIn[i].substr(3, 2)) + ((12 - (stoi(v.timeIn[i].substr(0, 2)) + 1)) * 60);
        if (stoi(v.timeOut[i].substr(0, 2)) == 12)
        {
            minutes += stoi(v.timeOut[i].substr(3, 2));
        }
        else
        {
            minutes += stoi(v.timeOut[i].substr(0, 2)) * 60;
            minutes += stoi(v.timeOut[i].substr(3, 2));
        }
    }
    else
    {
        if ((stoi(v.timeOut[i].substr(0, 2)) - stoi(v.timeIn[i].substr(0, 2))) < 0)
        {
            minutes = 250;
        }
        else if (v.timeOut[i].substr(0, 2) == v.timeIn[i].substr(0, 2))
        {
            if ((stoi(v.timeOut[i].substr(3, 2)) - stoi(v.timeIn[i].substr(3, 2))) < 0)
            {
                minutes = 250;
            }
            else
            {
                minutes += (stoi(v.timeOut[i].substr(3, 2)) - stoi(v.timeIn[i].substr(3, 2)));
            }
        }
        else
        {
            int l = (60 - stoi(v.timeIn[i].substr(3, 2)));
            int m = stoi(v.timeOut[i].substr(0, 2));
            int p = stoi(v.timeIn[i].substr(0, 2)) + 1;
            m = ((m - p) * 60);
            int o = (stoi(v.timeOut[i].substr(3, 2)));
            minutes += l + m + o;
        }
    }
    return minutes;
}
//Allocates slots to vehicle if available
bool System::checkin(string &s, Vehicles &v)
{
    int count = 0;
    int x = -1, y = -1;
    bool flag = true;
    for (int i = 0; i < matrix.size(); i++)
    {
        for (int j = 0; j < matrix[i].size(); j++)
        {
            if (matrix[i][j] == 0 && s == "NR")
            {
                matrix[i][j] = 2;
                x = i;
                y = j;
                flag = false;
                break;
            }
            else if (matrix[i][j] == 1 && s == "R")
            {
                matrix[i][j] = 3;
                x = i;
                y = j;
                flag = false;
                break;
            }
            if (matrix[i][j] == 3 && s == "R")
            {
                count++;
            }
            if (count == numberOfReserveSlots)
            {
                i = 0;
                j = -1;
                s = "NR";
                count = 0;
                cout << "Since all the reserved slots are full we will park this vehicle to Non-reservedslot\n";
            }
        }
        if (!flag)
        {
            break;
        }
    }
    if (x == -1 && y == -1)
    {
        return false;
    }
    char ch = x + 65;
    cout << ch << "-" << y + 1 << endl;
    string temp2 = to_string(y + 1);
    string temp;
    temp += ch;
    temp += "-";
    temp += temp2;
    v.slot.push_back(temp);
    return true;
}
//Free the occupied slot and calculate charge of keeping vehicle inside parking slot
void System::checkout(Vehicles &v,string timeOut,string numberPlate)
{
    if (v.numberPlate.size() == 0)
    {
        cout << "There is no vehicle in parking slot\n";
    }
    else
    {
        bool flag = false;
        int index;
        for (int i = v.numberPlate.size()-1; i >=0; i--)
        {
            if (numberPlate == v.numberPlate[i])
            {
                index = i;
                flag = true;
                v.timeOut[i] = timeOut;
                int minutes=calculateMinutes(v,i);
                if (minutes <= 120)
                {
                    v.charge[i] = 50;
                }
                else if (minutes > 120 && minutes <= 240)
                {
                    v.charge[i] = 80;
                }
                else
                {
                    v.charge[i] = 100;
                }
                cout << v.charge[i] << "\n";
                break;
            }
        }
        if (flag == false)
        {
            cout << "Vehicle not found\n";
        }
        else
        {
            int idx1 = v.slot[index][0] - 'A';
            int idx2 = (stoi(v.slot[index].substr(2))) - 1;
            if (matrix[idx1][idx2] == 2)
            {
                matrix[idx1][idx2] = 0;
            }
            else if (matrix[idx1][idx2] == 3)
            {
                matrix[idx1][idx2] = 1;
            }
        }
    }
}
//Generates the report of activities of a system
void System ::generateReport(Vehicles &v,vector<string>&report)
{
    cout << "PARKING SLOT, CAR NO, CHECK IN TIME, CHECK OUT TIME, CHARGES, CATEGORY\n";
    string temporary = "";
    for (int index = 0; index < v.timeIn.size(); index++)
    {
        if (v.charge[index] != 0)
        {
            temporary += v.slot[index] + "," + v.timeIn[index] + "," + v.numberPlate[index] + "," + v.timeOut[index] + "," + to_string(v.charge[index]) + "," + v.category[index];
            report.push_back(temporary);
            temporary = "";
        }
    }
    sort(report.begin(), report.end());
    for (int i = 0; i < report.size(); i++)
    {
        vector<string>ans2=splitByDelimeter(report[i],',');
        cout<<ans2[0]<<", "<<ans2[2]<<", "<<ans2[1]<<", "<<ans2[3]<<", "<<ans2[4]<<", "<<ans2[5]<<"\n";
    }
}
int main()
{
    System s;
    // Creating matrix where each entry is 0 or 1 or 2 or 3 as their value
    // 0 represents non reserved slots are not filled yet
    // 1 represents reserved slots are not filled yet
    // 2 represents non reserved slots are filled
    // 3 represents  reserved slots are filled

    // Creating matrix with all entry 0 initially
    for (int i = 0; i < s.n; i++)
    {
        vector<int> temp(s.x);
        for (int j = 0; j < s.x; j++)
        {
            temp[j] = 0;
        }
        s.matrix.push_back(temp);
    }
    //Updating matrix i.e making entry as 1 for reserved slots
    for (int i = 0; i < s.reservedSlots.size(); i++)
    {
        int x = (s.reservedSlots[i][0] - 'A');
        string str = s.reservedSlots[i].substr(2);
        int y = stoi(str) - 1;
        s.matrix[x][y] = 1;
    }
    Vehicles v;           //Contains information about vehicle
    vector<string>report; //Contains information about system
    while(true)
    {
        string input;
        getline(cin, input);
        vector<string> inp = splitByDelimeter(input,' ');
        if(inp[0]=="CHECKIN")
        {
            if (s.checkin(inp[3], v))
            {
                v.numberPlate.push_back(inp[1]);
                v.timeIn.push_back(inp[2]);
                v.category.push_back(inp[3]);
                v.timeOut.push_back("");
                v.charge.push_back(0);
            }
            else
            {
                cout << "PARKING FULL\n";
            }
            // print(v);
        }
        else if(inp[0]=="CHECKOUT")
        {
            s.checkout(v,inp[2],inp[1]);
            // print(v);
        }
        else
        {
            s.generateReport(v,report);
            break;
        }
    }
    return 0;
}