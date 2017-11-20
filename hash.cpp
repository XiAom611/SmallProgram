#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <fstream>
#include <cstdio>
using namespace std;


typedef struct Node
{
    string data;
    int frequence=1;
    struct Node* next=nullptr;
}Node;

int string_or_int(string input, int len)
{
    const char* input_string=input.data();
    int strorint=1;
    for (int i=0; i<len; i++)
    {
        if (input_string[i]!='0' && input_string[i]!='1' && input_string[i]!='2' && input_string[i]!='3' && input_string[i]!='4' && input_string[i]!='5' && input_string[i]!='6' && input_string[i]!='7' && input_string[i]!='8' && input_string[i]!='9')
        {
            strorint = 0;
            break;
        }
    }
    //cout << tmp->int_bool;
    return strorint;
}

int hash_int(string input, int length_table)
{
    unsigned int hash_value;
    hash_value = atoi(input.c_str()) % length_table;
    return hash_value;
}

int get_hashvalue(unsigned char c, int times, int length_table)
{
    int value = c % length_table;
    for (int i=0; i<times; i++)
    {
        value =  value*256 % length_table;
    }
    return value;
}

int hash_string(string input, int length_table)
{
    unsigned int hash_value=0;
    for (int i=0; i<input.length(); i++)
    {
        hash_value = hash_value + get_hashvalue(input[i], (int)input.length()-i-1, length_table);
    }
    return hash_value%length_table;
}

void print_hash_table(Node** table, int length_table)
{
    cout << "[" << endl;
    for (int i=0; i<length_table; i++)
    {
        int len=1;
        Node* tmp=table[i];
        cout << "{";
        if (tmp == NULL) {cout << "}";}
        else
        {
            while (tmp->next != nullptr) {len ++;tmp=tmp->next;}
            Node* buffer[len];
            tmp = table[i];
            for (int j=0; j<len; j++)
            {
                buffer[j] = tmp;
                tmp = tmp->next;
            }
            for (int j=len-1; j>=0; j--)
            {
                cout << "\"" << buffer[j]->data << "\":" << buffer[j]->frequence ;
                if (j != 0) {cout << ",";} else {cout << "}";}
            }
        }
        if (i != length_table-1) {cout << ","<<endl;} else {cout << endl;}
    }
    cout << "]" << endl;
}

int check_size(Node** table, int length, int lf)
{
    int increase;
    int size=0;
    for (int i=0; i<length; i++)
    {
        Node* tmp = table[i];
        if (tmp==NULL) {continue;}
        else
        {
            size ++;
            while(tmp->next != nullptr)
            {
                size ++;
                tmp = tmp->next;
            }
        }

    }
    if (size*100 > length*lf) {increase = 1;} else {increase = 0;}
    return increase;
}

Node** rehash(Node** table, int length)
{
    Node** table_new = new Node*[length];
    for (int i=0; i<length; i++) {table_new[i]=NULL;}
    for (int i=0; i<(length-1)/2; i++)
    {
        int len=1;
        Node* tmp = table[i];
        if (tmp==NULL) {continue;}
        else
        {
            while (tmp->next != nullptr) {len ++;tmp=tmp->next;}
            Node* buffer[len];
            tmp = table[i];
            for (int j=0; j<len; j++)
            {
                buffer[j] = tmp;
                tmp = tmp->next;
                //cout << buffer[j]->data << endl;
            }
            //rehash section
            for (int j=len-1; j>=0; j--)
            {
                // get hash_value
                Node* node_insert;
                node_insert = buffer[j];
                int strint = string_or_int(node_insert ->data, (int)node_insert->data.length());
                int hash_value;
                node_insert->next = nullptr;
                if (strint)
                {
                    hash_value = hash_int(buffer[j]->data, length);
                }
                else
                {
                    hash_value = hash_string(buffer[j]->data, length);
                }
                // do rehashing
                if(table_new[hash_value]==NULL)
                {
                    table_new[hash_value] = node_insert;
                }
                else
                {
                    tmp = table_new[hash_value];
                    while (tmp->next != nullptr)
                    {
                        tmp = tmp->next;
                    }
                    tmp->next = node_insert;
                }
            }
        }
    }
    delete []table;
    return table_new;
}

int check_in(string word_stop, string* word, int len)
{
    int same=0;
    for (int i=0; i<len; i++)
    {
        if (word_stop == word[i])
        {
            same = 1;
            break;
        }
    }
    return same;
}

int main(int argc, const char * argv[])
{
    int load_factor, init_length;
    string word_file, word_file_stop;

    load_factor = 75;
    init_length = 9;

    //get input stream
    for (int i=1; i<argc; i+=2)
    {
        if (strcmp(argv[i], "-f") == 0)
        {
            load_factor = atoi(argv[i+1]);
        }
        else if(strcmp(argv[i], "-l") == 0)
        {
            init_length = atoi(argv[i+1]);
        }
        else if(strcmp(argv[i], "-s") == 0)
        {
            word_file_stop = argv[i+1];
        }
        else if(strcmp(argv[i], "-w") == 0)
        {
            word_file = argv[i+1];
        }
    }
    ////test start////
    //cout << load_factor << " " << init_length << " " << word_file_stop << " " << word_file << endl;
    ////test end////

    //get word file
    string* word = new string [100000];
    ifstream in;
    in.open(word_file);
    char pre=EOF;
    int word_count = 0;
    char c = in.get();
    while (in.good())
    {
        if (c==' ' || c=='\n' || c=='\t' || c=='\r')
        {
            word_count ++;
            if (pre==' ' || pre =='\n' || pre=='\t' || pre=='\r' || pre==EOF)
            {
                word_count --;
            }
        }
        else
        {
            word[word_count]+=c;
        }

        pre = c;
        c = in.get();
    }
    if (pre!=' ' && pre !='\n' && pre!='\t' && pre!='\r' && pre!=EOF)
    {
        word_count ++;
    }
    in.close();
    //cout << word_count << endl;

  /*in.open(word_file);
    string tmp="";
    int tmp_count = 0;
    while (in.good())
    {
        c = in.get();
        if (c==' ' || c=='\n' || c=='\t' || c=='\r' || c==EOF)
        {
            if (tmp == "") {continue;}
            //cout << tmp << " " << tmp_count << endl;
            word[tmp_count] = tmp;
            tmp = "";
            tmp_count ++;
            continue;
        }
        tmp += c;
    }
    in.close();
*/
    // stop words
    string* word_stop = new string [100000];
    in.open(word_file_stop);
    pre=EOF;
    int word_stop_count = 0;
    c = in.get();
    while (in.good())
    {
        if (c==' ' || c=='\n' || c=='\t' || c=='\r')
        {
            word_stop_count ++;
            if (pre==' ' || pre =='\n' || pre=='\t' || pre=='\r' || pre==EOF)
            {
                word_stop_count --;
            }
        }
        else
        {
            word_stop[word_stop_count]+=c;
        }
        pre = c;
        c = in.get();
    }
    if (pre!=' ' && pre !='\n' && pre!='\t' && pre!='\r' && pre!=EOF)
    {
        word_stop_count ++;
    }
    in.close();
    //cout << word_stop_count << endl;
    /*
    in.open(word_file_stop);
    tmp="";
    tmp_count = 0;
    while (in.good())
    {
        c = in.get();
        if (c==' ' || c=='\n' || c=='\t' || c=='\r' || c==EOF)
        {
            if (tmp == "") {continue;}
            //cout << tmp << " " << tmp_count << endl;
            word_stop[tmp_count] = tmp;
            tmp = "";
            tmp_count ++;
            continue;
        }
        tmp += c;
    }
    in.close();*/
    //finish get input


    Node** hash_table = new Node*[init_length];
    for (int i=0; i<init_length; i++) {hash_table[i] = NULL;}

    //get hash value
    unsigned int hash_value;
    int size=0;
    for (int i=0; i<word_count; i++)
    {
        int strint = string_or_int(word[i], (int)word[i].length());
        //cout << word[i] << " " << strint[i]<< endl;
        if (strint)
        {
            hash_value = hash_int(word[i], init_length);
        }
        else
        {
            hash_value = hash_string(word[i], init_length);
        }

        Node* node_insert = new Node;
        node_insert->data = word[i];
        if (hash_table[hash_value] == NULL)
        {
            hash_table[hash_value] = node_insert;
            size ++;
        }
        else
        {
            Node* node_tmp = hash_table[hash_value];
            bool insertornot = 1;
            if (node_tmp->next == nullptr)
            {
                if (node_tmp->data == node_insert->data)
                {
                    //cout << "same" << endl;
                    node_tmp->frequence ++;
                    delete node_insert;
                    insertornot = 0;
                }
            }
            //cout << node_tmp->data << endl;
            while (node_tmp->next != nullptr)
            {
                if (node_tmp->data == node_insert->data)
                {
                    //cout << "same" << endl;
                    node_tmp->frequence ++;
                    delete node_insert;
                    insertornot = 0;
                    break;
                }
                node_tmp = node_tmp->next;
            }
            if (insertornot)
            {
                if (node_tmp->data == node_insert->data)
                {
                    node_tmp->frequence ++;
                    delete node_insert;
                    continue;
                }
                node_tmp->next = node_insert;
                size ++;
            }
        }
        if (size*100 > load_factor*init_length)
        {
            init_length = init_length*2 + 1;
            hash_table = rehash(hash_table, init_length);
        }
    }
    //////test start
    /*
    for (int i=0; i<init_length; i++)
    {
        Node* node_tmp = hash_table[i];
        cout << i << endl;
        if (node_tmp==NULL) {continue;}
        else
            {
                while (node_tmp->next != NULL)
                {
                    cout << node_tmp->data <<"("<<node_tmp->frequence<<")"<< "--";
                    node_tmp = node_tmp->next;
                }
                cout << node_tmp->data<<"("<<node_tmp->frequence<<")";
            }
        cout << endl;
    }
    for (int i=0; i<word_stop_count;i++)
        cout << word_stop[i];*/
    //////test end

    //delete

    for (int i=0; i<word_stop_count; i++)
    {
        int flag = 1;
        if (check_in(word_stop[i], word, word_count))
        {
            //do hashing
            int str_or_int=string_or_int(word_stop[i], (int)word_stop[i].length());
            if (str_or_int)
            {
                hash_value = hash_int(word_stop[i], init_length);
            }
            else
            {
                hash_value = hash_string(word_stop[i], init_length);
            }

            Node* cur = hash_table[hash_value];
            Node* pre = hash_table[hash_value];
            if(cur == NULL) {continue;}
            while (cur->data != word_stop[i])
            {
                pre = cur;
                cur = cur->next;
                if (cur == NULL)
                {
                    flag = 0;
                    break;
                }
                if (cur->data!=word_stop[i] && cur->next == nullptr)
                {
                    flag = 0;
                    break;
                }
            }
            if (flag == 0) {continue;}
            if (word_stop[i] == hash_table[hash_value]->data)
            {
                pre = hash_table[hash_value];
                hash_table[hash_value] = hash_table[hash_value]->next;
                delete pre;
                continue;
            }
            //cout << pre->data << " "<<cur->data<<endl;
            pre->next = cur->next;
            delete cur;
        }
    }
    //////test start
    /*
    for (int i=0; i<init_length; i++)
    {
        Node* node_tmp = hash_table[i];
        cout << i << endl;
        if (node_tmp==NULL) {continue;}
        else
        {
            while (node_tmp->next != NULL)
            {
                cout << node_tmp->data <<"("<<node_tmp->frequence<<")"<< "--";
                node_tmp = node_tmp->next;
            }
            cout << node_tmp->data<<"("<<node_tmp->frequence<<")";
        }
        cout << endl;
    }
    */
    //////test end

    print_hash_table(hash_table, init_length);

    return 0;
}
