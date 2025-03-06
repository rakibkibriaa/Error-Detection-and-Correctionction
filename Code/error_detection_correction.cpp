#include<iostream>
#include<vector>
#include<algorithm>
#include <windows.h>
#include<time.h>
using namespace std;

vector<bool> getBinary(int num)
{
    vector<bool>temp;

    while(num)
    {
        int r = num % 2;

        num = num / 2;

        temp.push_back(r);
    }

    int len = temp.size();
    for(int i=0;i<(8-len);i++)
    {
        temp.push_back(0);
    }

    reverse(temp.begin(),temp.end());

    return temp;
}
long long int getTwoPower(int m)
{
    long long int result = 1;
    for(int i=0;i<m;i++)
    {
        result = result * 2;
    }
    return result;
}

int getTotalCheckBits(long long int sz)
{
    /// 1 + m + r <= 2 ^ r

    int r = 0;

    while(sz + 1 > getTwoPower(r) - r)
    {
        r++;
    }
    return r;
}
int updateCheckBit(int pos,vector<bool>& v)
{

    int ind = pos - 1;
    int val = v[pos - 1];

    while(ind < v.size())
    {
        int i;
        for(i=0;i<pos;i++)
        {
            if(ind + i < v.size())
            {
                val = val ^ v[ind + i];
                //cout<<ind + i<<" ";
            }
        }

        ind += (pos + i);
    }

    if(val == 1)
    {
        v[pos - 1] = 1;
    }

}
string getStringFromBinary(vector<bool> message)
{
    string temp = "";

    int ind = 0;

    int start = 0;

    while(ind < message.size())
    {
        int sum = 0;

        for(int i=ind;i<ind + 8;i++)
        {
            sum = sum * 2 + message[i];
        }

        ind += 8;

        temp += sum;
    }

    return temp;

}

vector<bool> getCRCRemainder(vector<bool>divisor,vector<bool>dividend)
{
    while(divisor.size() > 0)
    {
        if(divisor[0] == 0)
        {
            divisor.erase(divisor.begin());
        }
        else
        {
            break;
        }
    }

    int index = divisor.size();

    for(int i=0;i<divisor.size() - 1;i++)
    {
        dividend.push_back(0);
    }

    //prune leading 0
    while(dividend.size() > 0)
    {

        if(dividend[0] == 0)
        {
            dividend.erase(dividend.begin());
        }
        else
        {
            break;
        }
    }

    vector<bool> temp_div = dividend;

    while(index < dividend.size())
    {

        if(temp_div[0] == 1)
        {
            vector<bool>temp;

            for(int i=0;i<divisor.size();i++)
            {
                temp.push_back(divisor[i] ^ temp_div[i]);
            }

            temp.push_back(dividend[index]);

            temp.erase(temp.begin());

            temp_div = temp;

            index++;

        }
        else
        {
            temp_div.push_back(dividend[index]);
            index++;


            temp_div.erase(temp_div.begin());
        }

    }

    if(temp_div[0] == 1)
    {
        vector<bool>temp;

        for(int i=0;i<divisor.size();i++)
        {
            temp.push_back(divisor[i] ^ temp_div[i]);
        }

        temp_div = temp;

    }

    temp_div.erase(temp_div.begin());

    return temp_div;
}

void correctErrrorBit(vector<bool>& v)
{
    int sum = 0;
    for(int pos=1;pos<v.size();pos=pos*2)
    {

        int ind = pos - 1;
        int val = v[pos - 1];

        while(ind < v.size())
        {
            int i;
            for(i=0;i<pos;i++)
            {
                if(ind + i < v.size())
                {
                    val = val ^ v[ind + i];

                }
            }

            ind += (pos + i);
        }

        if(val ^ v[pos - 1] == 1)
        {
            sum += pos;

        }
    }

    if(sum >=1 && sum <= v.size())
    {
        v[sum - 1] = 1 - v[sum - 1];
    }

}
int main()
{
    string s;
    string gen;
    int m;

    double p;

    ///srand(time(NULL));

    cout<<"enter data string: ";
    getline(cin, s);

    cout<<"enter number of data bytes in a row (m): ";
    cin >> m;
    cout<<"enter probability (p): ";
    cin >> p;
    cout<<"enter generator polynomial: ";
    cin >> gen;

    vector<bool>generator;

    for(int i=0;i<gen.size();i++)
    {
        generator.push_back(gen[i] - '0');
    }
    if(s.size() % m != 0)
    {
        int diff = (s.size() / m + 1) * m - s.size();

        for(int i=0;i<diff;i++)
        {
            s += '~';
        }
    }
    cout<<"\n";
    cout<<"data string after padding: "<<s<<endl;

    int total_rows = s.size() / m;

    vector<vector<bool>>row_major(total_rows);

    int ind = 0;

    int counter = 0;

    for(int i=0;i<s.size();i++)
    {

        vector<bool> temp = getBinary(s[i]);

        for(int i=0;i<temp.size();i++)
        {
            row_major[ind].push_back(temp[i]);
        }

        counter++;

        if(counter % m == 0)
        {
            ind++;
        }
    }

    cout<<endl;
    cout<<"data block (ascii code of m characters per row): "<<endl;

    for(int i=0;i<row_major.size();i++)
    {
        for(int j=0;j<row_major[i].size();j++)
        {
            cout<<row_major[i][j]<<"";
        }
        cout<<endl;
    }

    int total_check_bits = getTotalCheckBits(m * 8);

    vector<vector<bool>>dummy_bit_added(total_rows);

    for(int i=0;i<row_major.size();i++)
    {
        int j = 0;
        int pow = 1;
        int counter = 1;

        while(j < row_major[i].size())
        {
            if(pow == counter)
            {
                pow = pow * 2;

                counter++;

                dummy_bit_added[i].push_back(0);
                continue;

            }
            dummy_bit_added[i].push_back(row_major[i][j]);
            j++;
            counter++;
        }
    }

    for(int i=0;i<dummy_bit_added.size();i++)
    {
        long long int counter = 1;
        long long int pow = 1;

        for(int j=0;j<dummy_bit_added[i].size();j++)
        {
            if(counter == pow)
            {
                updateCheckBit(counter,dummy_bit_added[i]);

                pow = pow * 2;

            }

            counter++;
        }
    }

    cout<<endl;
    cout<<"data blocks after adding check bits: "<<endl;

    for(int i=0;i<dummy_bit_added.size();i++)
    {
        int pow = 1;
        int counter = 1;
        for(int j=0;j<dummy_bit_added[i].size();j++)
        {
            if(pow == counter)
            {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
                pow = pow * 2;
            }
            else
            {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
            }

            cout<<dummy_bit_added[i][j]<<"";
            counter++;
        }
        cout<<endl;
    }
    vector<bool> column_wise_serialization;

    for(int i=0;i<dummy_bit_added[0].size();i++)
    {
        for(int j=0;j<total_rows;j++)
        {
            column_wise_serialization.push_back(dummy_bit_added[j][i]);
        }
    }

    cout<<endl;

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
    cout<<"data bits after column-wise serialization:"<<endl;

    int prevSize = column_wise_serialization.size();

    for(int i=0;i<column_wise_serialization.size();i++)
    {

        cout<<column_wise_serialization[i]<<"";
    }
    cout<<endl;

    vector<bool> checkSum = getCRCRemainder(generator,column_wise_serialization);

    for(int i=0;i<checkSum.size();i++)
    {

        column_wise_serialization.push_back(checkSum[i]);

    }
    cout<<endl;
    cout<<"data bits after appending CRC checksum (sent frame):"<<endl;
    for(int i=0;i<column_wise_serialization.size();i++)
    {
        if(i >= prevSize)
        {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
        }
        else
        {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
        }
        cout<<column_wise_serialization[i]<<"";
    }

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);

    vector<bool>received_frame;

    vector<bool>error_position;

    for(int i=0;i<column_wise_serialization.size();i++)
    {
         double random =  (1.0 * rand()) / RAND_MAX;

         if(random <= p)
         {
            error_position.push_back(1);

            received_frame.push_back(1 - column_wise_serialization[i]);
         }
         else
         {
             error_position.push_back(0);

             received_frame.push_back(column_wise_serialization[i]);
         }
    }

    cout<<endl;
    cout<<endl;
    cout<<"received frame:"<<endl;

    for(int i=0;i<received_frame.size();i++)
    {
        if(error_position[i] == 1)
        {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);

        }
        else
        {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
        }
        cout<<received_frame[i]<<"";
    }
    cout<<endl;

    cout<<endl;

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);

    cout<<"result of CRC checksum matching: ";

    vector<bool> result = getCRCRemainder(generator,received_frame);

    bool flag = 0;
    for(int i=0;i<result.size();i++)
    {
        if(result[i] == 1)
        {
            flag = 1;
            break;
        }
    }

    if(flag)
    {
        cout<<"error detected"<<endl;
    }
    else
    {
        cout<<"no error detected"<<endl;
    }

    cout<<endl;

    vector<vector<bool>> removed_checksum_bits(total_rows);

    for(int i=0;i<(received_frame.size() - result.size());i++)
    {
        removed_checksum_bits[i % total_rows].push_back(received_frame[i]);
    }

    int pos = 0;

    cout<<"data block after removing CRC checksum bits:"<<endl;
    for(int i=0;i<removed_checksum_bits.size();i++)
    {
        for(int j=0;j<removed_checksum_bits[i].size();j++)
        {
            if(error_position[j * total_rows + i] == 1)
            {
                 SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
            }
            else
            {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
            }
            cout<<removed_checksum_bits[i][j]<<"";
        }
        cout<<endl;
    }
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);

    ///Error correction

    for(int i=0;i<removed_checksum_bits.size();i++)
    {
        correctErrrorBit(removed_checksum_bits[i]);
    }

    vector<vector<bool>> removed_check_bits(total_rows);


    for(int i=0;i<removed_checksum_bits.size();i++)
    {
        long long int pow = 1;
        long long int counter = 1;

        for(int j=0;j<removed_checksum_bits[i].size();j++)
        {
            if(counter == pow)
            {
                pow = pow * 2;
                counter++;
                continue;
            }

            removed_check_bits[i].push_back(removed_checksum_bits[i][j]);

            counter++;

        }
    }
    cout<<endl;
    cout<<"data block after removing check bits:"<<endl;
    for(int i=0;i<removed_check_bits.size();i++)
    {
        for(int j=0;j<removed_check_bits[i].size();j++)
        {
            cout<<removed_check_bits[i][j]<<"";
        }
        cout<<endl;
    }
    cout<<endl;


    string output_frame="";

    for(int i=0;i<removed_check_bits.size();i++)
    {
        output_frame += getStringFromBinary(removed_check_bits[i]);
    }
    cout<<"output frame: ";
    cout<<output_frame<<endl;
    return 0;
}
