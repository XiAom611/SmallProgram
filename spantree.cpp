#include <iostream>
using namespace std;

typedef struct ROAD
{
    int c1, c2, length, region;
}ROAD;

typedef struct _ROAD
{
    ROAD road;
    struct _ROAD* next;
    struct _ROAD* tail;
}_ROAD;

typedef struct REGION
{
    int region, root, sc, count, num;
}REGION;

typedef struct ROOT
{
    int root;
    int num;
}ROOT;

void quick_sort(ROAD r[], int low, int high)
{
    if (low >= high) {return;}
    int left = low;
    int right = high;
    ROAD key = r[left];
    while (left < right)
    {
        while ((left<right&&r[right].length>key.length) || (left<right&&r[right].length==key.length&&r[right].c1==key.c1&&r[right].c2>key.c2) || (left<right&&r[right].length==key.length&&r[right].c1>key.c1))
        {
            right --;
        }
        r[left] = r[right];
        while ((left<right && r[left].length<key.length) || (left<right&&r[left].length==key.length&&r[left].c1==key.c1&&r[left].c2<key.c2) || (left<right&&r[left].length==key.length&&r[left].c1<key.c1))
        {
            left ++;
        }
        r[right] = r[left];
    }
    r[left] = key;
    quick_sort(r, low, left-1);
    quick_sort(r, left+1, high);
}

int find_father(REGION r[], int c)
{
    while (r[c].region != c)
    {
        c = r[c].region;
    }
    return c;
}

void quick_sort_region(ROOT r[], int low, int high, REGION re[])
{
    if (low >= high) {return;}
    int left = low;
    int right = high;
    ROOT key = r[left];
    while (left < right)
    {
        while ((left<right&&re[r[right].root].count>re[key.root].count) || (left<right&&re[r[right].root].count==re[key.root].count&&re[r[right].root].sc>re[key.root].sc))
        {
            right --;
        }
        r[left] = r[right];
        while ((left<right&&re[r[left].root].count<re[key.root].count) || (left<right&&re[r[left].root].count==re[key.root].count&&re[r[left].root].sc<re[key.root].sc))
        {
            left ++;
        }
        r[right] = r[left];
    }
    r[left] = key;
    quick_sort_region(r, low, left-1, re);
    quick_sort_region(r, left+1, high, re);
}

int main(int argc, const char * argv[])
{
    int city_num, road_num;
    cin >> city_num >> road_num;
    ROAD road[road_num];
    for (int i=0; i<road_num; i++)
    {
        cin >> road[i].c1 >> road[i].c2 >> road[i].length;
        if (road[i].c1 > road[i].c2) {swap(road[i].c1, road[i].c2);}
    }
    quick_sort(road, 0, road_num-1);
    ////////////////////test start////////////////////
    cout << "TEST1 START" << endl;
    for (int i=0; i<road_num; i++)
    {
        cout<<road[i].c1<<" "<<road[i].c2<<" "<<road[i].length<< endl;
    }
    cout << "TEST1 END" << endl;
    ////////////////////test end////////////////////

    REGION region[city_num];
    for (int i=0; i<city_num; i++)                                          // init region
    {
        region[i].region = i;
        region[i].root = i;
        region[i].sc = i;
        region[i].count = 0;
        region[i].num = -1;
    }
    _ROAD* road_list = new _ROAD;                                           // the road being printed
    road_list = nullptr;
    int road_num_eff = 0;
    for (int i=0; i<road_num; i++)
    {
        int f1, f2;
        f1 = find_father(region, road[i].c1);
        f2 = find_father(region, road[i].c2);
        if (f1 != f2)                                                       // union set
        {
            road_num_eff ++;
            region[f2].region = region[f1].region;
            region[f2].root = region[f1].root;
            _ROAD* road_want = new _ROAD;
            road_want->road = road[i];
            road_want->next = road_list;
            road_list = road_want;
        }
        else
        {
            continue;
        }
    }
    int region_num = 0;
    for (int i=0; i<city_num; i++)                                          // find root and how many cities in the region
    {
        while (region[i].root != region[region[i].root].root)
        {
            region[i].root = region[region[i].root].root;
        }
        if (region[i].root > i)
        {
            region[region[i].root].sc= i;
        }
        region[region[i].root].count ++ ;
        if (region[i].count != 0)
        {
            region[i].num = region_num;
            region_num ++;
        }
    }
    ////////////////////test start////////////////////
    cout << "TEST2 START" << endl;
    cout << road_num_eff<<endl;
    /*while (road_list!=nullptr)
    {
        cout << road_list->road.c1<<" "<<road_list->road.c2<<" "<<road_list->road.length<<endl;
        road_list = road_list->next;
    }*/
    for (int i=0; i<city_num; i++)
        cout<<region[i].region << " " ;
    cout << endl;
    for (int i=0; i<city_num; i++)
        cout<<region[i].root << " " ;
    cout << endl;
    for (int i=0; i<city_num; i++)
        cout<<region[i].sc << " " ;
    cout << endl;
    for (int i=0; i<city_num; i++)
        cout<<region[i].count << " " ;
    cout << endl;
    for (int i=0; i<city_num; i++)
        cout<<region[i].num << " " ;
    cout << endl;
    cout << region_num << endl;
    cout << "TEST2 END" << endl;
    ////////////////////test end////////////////////
    ROOT roots[region_num];
    int tmp_count = 0;
    for (int i=0; i<city_num; i++)
    {
        if (region[i].count != 0)
        {
            roots[tmp_count].root = i;
            roots[tmp_count].num = tmp_count;
            tmp_count++;
        }
        if (tmp_count == region_num)
        {
            break;
        }
    }
    _ROAD** road_region = new _ROAD*[region_num];
    for (int i=0; i<region_num; i++)
    {
        road_region[i] = nullptr;
    }
    while (road_list != nullptr)
    {
        _ROAD* cur = road_list;
        road_list = road_list->next;
        cur->next = nullptr;
        if (road_region[region[region[cur->road.c1].root].num] == nullptr)
        {
            road_region[region[region[cur->road.c1].root].num] = cur;
            road_region[region[region[cur->road.c1].root].num]->tail = cur;
        }
        else
        {
            road_region[region[region[cur->road.c1].root].num]->tail->next = cur;
            road_region[region[region[cur->road.c1].root].num]->tail = road_region[region[region[cur->road.c1].root].num]->tail->next;
        }
    }
    delete road_list;
    ////////////////////test start////////////////////
    cout << "TEST3 START"<<endl;
    /*for (int i=0; i<region_num; i++)
    {
        while (road_region[i] != nullptr)
        {
            cout << road_region[i]->road.c1 <<" "<<road_region[i]->road.c2<<" "<<road_region[i]->road.length<<endl;
            road_region[i] = road_region[i]->next;
        }
    }*/
    cout <<"TEST3 END"<<endl;
    ////////////////////test end////////////////////
    quick_sort_region(roots, 0, region_num-1, region);
    ////////////////////test start////////////////////
    cout << "TEST4 START"<<endl;
    for (int i=0; i<region_num; i++)
    {
        cout << roots[i].num << " "  << roots[i].root << endl;
    }
    cout <<endl;
    cout <<"TEST4 END"<<endl;
    ////////////////////test end////////////////////
    cout << "[" << endl;                                                    // output
    for (int i=region_num-1; i>=0; i--)
    {
        cout << "[" <<endl;
        while (road_region[roots[i].num] != nullptr)
        {
            if (road_region[roots[i].num]->next == nullptr)
            {
                cout << "[" << road_region[roots[i].num]->road.c1 << "," << road_region[roots[i].num]->road.c2 << "," << road_region[roots[i].num]->road.length << "]"<<endl;
                break;
            }
            cout << "[" << road_region[roots[i].num]->road.c1 << "," << road_region[roots[i].num]->road.c2 << "," << road_region[roots[i].num]->road.length << "],"<<endl;
            road_region[roots[i].num] = road_region[roots[i].num]->next;
        }
        if (i==0)
        {
            cout << "]" << endl;
        }
        else
        {
            cout << "]," << endl;
        }
    }
    cout << "]" << endl;

    return 0;
}
