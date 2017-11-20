#include <iostream>
#include <stdio.h>
#include <string.h>
#include <string>
using namespace std;

unsigned int allc[256];
const char magiccookie[] = {"HUFFMAN"};
int len;
int q_cur_len = 0;

class Node
{
public:
    char character;
    int weight;
    Node* child_left;
    Node* child_right;
    string b_code;
    int ascii;
    Node(char c, int w, Node* l, Node* r, string code, int asc)
    {
        character = c;
        weight = w;
        child_left = l;
        child_right = r;
        b_code = code;
        ascii = asc;
    }
};

typedef struct dic
{
    unsigned char c;
    string code;
}dic;

int *init_input()
{
    int count=0;
    int *f = new int[len*2];
    for (int i=0; i<=255; i++)
    {
        if (allc[i] != 0)
        {
            f[count] = i;
            f[count+1] = allc[i];
            count += 2 ;
        }
    }
    return f;
}

void bubble_sort(Node** list,int start, int n)
{
    for (int i=start; i<n; i++)
    {
        for (int j=i+1; j<n; j++)
        {
            if (list[i]->weight > list[j]->weight)
            {
                Node* tmp = list[i];
                list[i] = list[j];
                list[j] = tmp;
            }
            else if(list[i]->weight == list[j]->weight)
            {
                int min_cur=list[i]->ascii;
                int min_next=list[j]->ascii;
                if (min_cur > min_next)
                {
                    Node* tmp = list[i];
                    list[i] = list[j];
                    list[j] = tmp;
                }
            }
        }
    }
}

void build_tree(unsigned char* c, int* f,int n, Node** arr)
{
    for (int i=0; i<n; i++)
    {
        arr[i] = new Node(c[i], f[i], nullptr, nullptr, "", (unsigned char)c[i]);
    }
    bubble_sort(arr, 0, n);
    int tmp = 0;
    while (tmp != n-1)
    {
        Node* min_r = arr[tmp];
        Node* min_l = arr[tmp+1];
        Node* node_new = new Node(' ', min_r->weight + min_l->weight, min_l, min_r, "",min(min_r->ascii,min_l->ascii));
        arr[tmp+1] = node_new;
        tmp ++;
        bubble_sort(arr, tmp, n);
    }
}

void add(Node* cur, Node** queue)
{
    queue[q_cur_len++] = cur;
}

Node* pop(Node** queue)
{
    Node* tmp = queue[0];
    q_cur_len --;
    for (int i=1; i<=q_cur_len; i++)
    {
        queue[i-1] = queue[i];
    }
    return tmp;
}

bool notempty(Node** queue)
{
    return (q_cur_len!=0);
}

dic* build_code(Node* tree, Node** queue)
{
    add(tree, queue);
    int i=0;
    dic* dic_code = new dic[len];
    //cout << queue[0]->weight << "!!!"<<q_cur_len<<endl;
    while (notempty(queue))
    {
        //cout << "!!" << q_cur_len<<"!!"<<endl;
        Node* cur = pop(queue);
        Node* lc = cur->child_left;
        Node* rc = cur->child_right;
        if (lc != nullptr)
        {
            lc->b_code = cur->b_code + "0";
            add(lc, queue);
        }
        if (rc != nullptr)
        {
            rc->b_code = cur->b_code + "1";
            add(rc, queue);
        }
        if (lc==nullptr && rc==nullptr)
        {
            //cout << cur->character << ":"<<cur->b_code<<endl;
            dic_code[i].c = (unsigned char)(int)cur->character;
            dic_code[i].code = cur->b_code;
            //cout <</*(int)(unsigned char)dic_code[i].c <<":"<<*/ dic_code[i].code<<endl;
            i++;
        }
    }
    return dic_code;
}

string compress(dic* dic_code, char input_allc[], long length)
{
    string output="";
    for (int i=0; i<length; i++)
    {
        unsigned char c = (unsigned char)(int)input_allc[i];
        //cout <<i<<" "<< c<<endl;
        for (int j=0; j<len; j++)
        {
            if (c == dic_code[j].c)
            {
                //cout << "**********"<< j<<endl;
                output += dic_code[j].code;
            }
        }
    }
    while(output.length()%8 != 0)
    {
        output += "0";
    }
    return output;
}

int pow(int x, int y)
{
    int re = x;
    if (y == 0)
    {
        return 1;
    }
    for (int i=1; i<y; i++)
    {
        re = re*x;
    }
    return re;
}

int b_string2int(char binarycode[])
{
    int dec = 0;
    for (int i=0; i<8; i++)
    {
        if (binarycode[i] == '1')
        {
            dec += pow(2,7-i);
        }
    }
    return dec;
}

void print_compressed_data(string compressed)
{
    char c[compressed.length()];
    strcpy(c, compressed.c_str());
    for (int i=0; i<compressed.length()/8; i++)
    {
        int k=0;
        char tmp[8];
        for (int j=(i+1)*8-1; j>=i*8; j--)
        {
            //cout << c[j];
            tmp[k] = c[j];
            //cout << tmp[k];
            k++;
        }
        cout << (char)b_string2int(tmp);
    }
}

void print_frequence()
{
    for (int i=0; i<256; i++)
    {
        unsigned int cur = allc[i];
        unsigned int o1=0, o2=0, o3=0, o4=0;
        int l1=pow(2, 7)-1, l2=pow(2, 15)-1, l3=pow(2, 23)-1, l4=pow(2, 31)-1;
        if (cur <= l1)
        {
            o1 = cur;
        }
        else if (cur>l1 && cur<=l2)
        {
            o2 = cur/((l1+1)*2);
            o1 = cur - o2*(l1+1)*2;
        }
        else if (cur>l2 && cur<l3)
        {
            o3 = cur/((l2+1)*2);
            o2 = (cur - o3*(l2+1)*2)/((l1+1)*2);
            o1 = cur - o3*(l2+1)*2 - o2*(l1+1)*2;
        }
        else if (cur>=l3 && cur<l4)
        {
            o4 = cur/((l3+1)*2);
            o3 = (cur - o4*(l3+1)*2)/((l2+1)*2);
            o2 = (cur - o4*(l3+1)*2 - o3*(l2+1)*2)/((l1+1)*2);
            o1 = cur - o4*(l3+1)*2 - o3*(l2+1)*2 - o2*(l1+1)*2;
        }
        cout << char(o1) << char(o2) << char(o3) << char(o4);
    }
}

int get_frequence(int f1, int f2, int f3, int f4)
{
    int f = 0;
    if (f2==0 && f3==0 && f4==0)
    {
        f += f1;
    }
    else if (f3==0 && f4==0)
    {
        f += f1;
        f += f2*pow(2,8);
    }
    else if (f4==0)
    {
        f += f1;
        f += f2*pow(2,8);
        f += f3*pow(2,16);
    }
    else
    {
        f += f1;
        f += f2*pow(2,8);
        f += f3*pow(2,16);
        f += f4*pow(2,24);
    }
    //cout <<"!!"<<f<<endl;
    return f;
}

string dec2bin(int dec)
{
    string rbin="";
    if (dec==0) {rbin+="0";}
    while (dec != 0)
    {
        if (dec%2 == 0) {rbin += "0";}
        else {rbin += "1";}
        dec = dec/2;
    }
    while (rbin.length() != 8)
    {
        rbin += "0";
    }
    return rbin;
}

void uncompressed(char* data, dic* dictionary, int counter)
{
    string tmp = "";
    int j = 0;
    while (counter != 0)
    {
        for (int i=0; i<len; i++)
        {
            if (dictionary[i].code == tmp)
            {
                cout << dictionary[i].c;
                tmp = "";
                counter --;
            }
        }
        tmp += data[j];
        j ++;
    }
}



int main(int argc, char *argv[])
{
    //freopen("/Users/moxiaoye/hw2/hw2/O10w", "r", stdin);
    if (argc == 1)
    {
        int input_c = getchar();
        string input_raw;
        for (int i=0; i<=255; i++)
        {
            allc[i] = 0;
        }
        while (input_c != EOF)
        {
            unsigned int c = (unsigned char)input_c;
            allc[c] += 1;
            input_raw += input_c;
            input_c = getchar();
        }
        for (int i=0; i<=255; i++)
        {
            if (allc[i] != 0)
            {
                len++;
            }
        }
        char* input_allc = (char*)input_raw.data();
        int *input_c_f;
        input_c_f = init_input();
        unsigned char list_c[len];
        int list_f[len];
        Node* arr[len];
        for (int i=0; i<len*2; i+=2)
        {
            list_c[i/2] = (char)input_c_f[i]; //just character
            list_f[i/2] = input_c_f[i+1]; // used character's frequence
        }
    
        build_tree(list_c, list_f, len, arr);
        Node* queue[len];
        dic* dic_code = build_code(arr[len-1], queue);
        string compressed_data = compress(dic_code, input_allc, input_raw.length());
        cout << magiccookie << char(0);
        print_frequence();
        print_compressed_data(compressed_data);
        }
    else if (argc==2 && argv[1][0]=='-' && argv[1][1]=='d')
    {
        int input_c;
        for (int i=0; i<8; i++)
        {
            input_c = (unsigned char)getchar();
        }
        int count = 0;
        for (int i=0; i<256*4; i+=4)
        {
            int f1 = (unsigned char)getchar();
            int f2 = (unsigned char)getchar();
            int f3 = (unsigned char)getchar();
            int f4 = (unsigned char)getchar();
            allc[i/4] = get_frequence(f1, f2, f3, f4);
            //cout <<i/4 <<"**" << allc[i/4]<<endl;
        }
        for (int i=0; i<=255; i++)
        {
            if (allc[i] != 0)
            {
                len++;
                count += allc[i];
            }
        }
        int *input_c_f;
        input_c_f = init_input();
        unsigned char list_c[len];
        int list_f[len];
        Node* arr[len];
        for (int i=0; i<len*2; i+=2)
        {
            list_c[i/2] = (char)input_c_f[i]; //just character
            list_f[i/2] = input_c_f[i+1]; // used character's frequence
            //cout <<"---"<< list_c[i/2]<<":"<<list_f[i/2] << endl;
        }
        build_tree(list_c, list_f, len, arr);
        Node* queue[len];
        dic* dic_code = build_code(arr[len-1], queue);
        string bin = "";
        while ((input_c=getchar()) != EOF)
        {
            bin += dec2bin((unsigned char)input_c);
        }
        //cout << bin<<endl;
        char* input_allc = (char*)bin.data();
        //cout << input_allc<<endl;
        uncompressed(input_allc, dic_code, count);
    }
    return 0;
}