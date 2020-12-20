#include <iostream>
#include <map>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
using namespace std;
map<char,int> score = {{'1', 1}, {'2', 0}, {'3', 1}, {'4', 2}, {'5', 3}, {'6', 4},
                       {'7', 5}, {'8', 6}, {'9', 7}, {'T', 8}, {'J', 9}, {'Q', 10}, {'K', 11}, {'A', 12}};
int pow(int a,int b){
    int result = 1;
    for (int i = 1; i<=b; ++i){
        result*=a;
    }
    return result;
}
class PokerHand {
public:
    enum comb{
        STRAIGHT_FLUSH, FOUR_OF_A_KIND,FULL_HOUSE,FLUSH,STRAIGHT,THREE_OF_A_KIND,TWO_PAIRS,PAIR,HIGH_CARD
    };
    static bool fiveStraight(const vector<pair<char,char>>& a){
        for (int i =0; i<a.size() - 1; ++i){
            if (i == 3 && a[i + 1].first == 'A' && a[0].first == '2') return true;
            else if (!(score[a[i + 1].first] - score[a[i].first] == 1)) return false;
        }
        return true;
    }
    static void sortByRank(vector<pair<char,char>>& a){
        sort(a.begin(), a.end(), [](pair<char,char> x, pair<char,char> y){
            return score[x.first] < score[y.first];
        });
        return;
    }
    static void sortByPairs(vector<pair<char,char>>&a){
        map<char,int> cC;
        for (auto c : a){
            cC[c.first]++;
        }
        sort(a.begin(), a.end(), [&cC](pair<char,char> x, pair<char,char> y){
           if (cC[x.first] == cC[y.first]) return score[x.first] < score[y.first];
           else return cC[x.first] < cC[y.first];
        });
    }
    static pair<int,int> getPairs(vector<pair<char,char>>& a){
        int first=1,second = 0, third = 0;
        for (int i = 0; i< a.size() - 1; ++i){
            if (a[i].first == a[i + 1].first) {
                if (first == 1) third+=2;
                else third++;
                first++;
                if(first == 2)++second;
            }else{
                first = 1;
            }
        }
        return make_pair(second, third);
    }
    static bool fiveSuits(vector<pair<char,char>>& a){
        for (int i =0; i<a.size() - 1; ++i){
            if (a[i].second != a[i + 1].second) return false;
        }
        return true;
    }
    static comb getComb(vector<pair<char,char>>& a){
        bool Suits = fiveSuits(a);
        bool Straight = fiveStraight(a);
        if (Suits && Straight) return STRAIGHT_FLUSH;
        if (Straight) return  STRAIGHT;
        pair<int,int> checkPair = getPairs(a);
        if(checkPair.first == 1 && checkPair.second == 4) return FOUR_OF_A_KIND;
        if(checkPair.first == 2 && checkPair.second == 5) return FULL_HOUSE;
        if(Suits) return FLUSH;
        if(checkPair.first == 1 && checkPair.second == 3) return THREE_OF_A_KIND;
        if(checkPair.first == 2 && checkPair.second == 4) return TWO_PAIRS;
        if(checkPair.first == 1 && checkPair.second == 2) return PAIR;
        return HIGH_CARD;
    }
    static int getStrength(string s) {
        int result = 1;
        istringstream iss(s);
        string v;
        vector<pair<char,char>> temp;
        while(iss >> v){
            for (int i= 0 ; i<5; i++)
                temp.emplace_back(v[i*2],v[i*2 + 1]);
        }
        sortByRank(temp);
        int comb = getComb(temp);
        sortByPairs(temp);
        result<<=(28 - comb);
        if ((temp[temp.size() - 1].first == 'A' && comb == 0 && temp[0].first == '2') || (temp[temp.size() - 1].first == 'A' && comb == 4 && temp[0].first == '2')) { //low ace case
            temp[0].first = '1';temp[1].first = '2';temp[2].first = '3';temp[3].first = '4';temp[4].first = '5';
        }
        for (int i= temp.size() - 1; i>=0; --i){
            result+=(pow(13,i)*score[temp[i].first]);
        }
        return result;
    }
    static bool next_combination (vector<int> & a, int n) {
        int k = (int)a.size();
        for (int i=k-1; i>=0; --i)
            if (a[i] < n-k+i+1) {
                ++a[i];
                for (int j=i+1; j<k; ++j)
                    a[j] = a[j-1]+1;
                return true;
            }
        return false;
    }
    static int getBestStrength(string s, string d){
        string cards = d + s;
        vector<int> perm = {1,2,3,4,5};
        int maxResult = 0;
        do{
            string f="";
            for (auto v : perm){
                f+=string(1,cards[(v -1)*2])+string(1,cards[(v-1)*2 + 1]);
            }
            maxResult = max(maxResult,getStrength(f));
        }while(next_combination(perm, 7));
        return maxResult;
    }
    static int getBestOmaha(string s, string d){
        vector<int> permBoard = {1,2,3};
        vector<int> permHand = {1,2};
        int maxResult = 0;
        int count = 1;
        do{
            string f = "";
            for (auto v : permBoard){
                f+=string(1,s[(v -1)*2])+string(1,s[(v-1)*2 + 1]);
            }
          do{
              string ff = f;
              for (auto v : permHand){
                  ff+=string(1,d[(v -1)*2])+string(1,d[(v-1)*2 + 1]);
              }
              maxResult = max(maxResult,getStrength(ff));
          }while(next_combination(permHand,4));
          permHand = {1,2};
        }while(next_combination(permBoard,5));
        return maxResult;
    }
    static string print(vector<pair<string,int>>& wr) {
        string print;
        vector<pair<string,int>> forSort;
        for (int i = 0; i<wr.size(); ++i){
            if (forSort.size() && forSort.back().second == wr[i].second)
                forSort.emplace_back(wr[i]);
            else{
                sort(forSort.begin(), forSort.end(), [](auto x, auto y){
                    return x.first < y.first;
                });
                for (int i = 0; i<(int)forSort.size()- 1; ++i){
                    print+=forSort[i].first + "=";
                }
                if(forSort.size()) {
                    print += forSort[forSort.size() - 1].first + " ";
                }
                forSort.clear();
                forSort.emplace_back(wr[i]);
            }

        }
        sort(forSort.begin(), forSort.end(), [](auto x, auto y){
            return x.first < y.first;
        });
        for (int i = 0; i<(int)forSort.size()- 1; ++i){
            print+=forSort[i].first + "=";
        }
        if(forSort.size()) {
            print += forSort[forSort.size() - 1].first;
        }
        return print;
    }

};
string Compare(string s, string v){
    return (PokerHand::getStrength(s) == PokerHand::getStrength(v)? "TIE" :
           PokerHand::getStrength(s) > PokerHand::getStrength(v) ? "FIRST" : "SECOND");
}
int main() {
    freopen("input.txt", "r", stdin);
    string line;
    while(getline(cin, line)){
        istringstream iss(line);
        string g_;
        iss >> g_;
        if (g_ == "five-card-draw"){
            vector<pair<string,int>> hands;
            string hand;
            while(iss >> hand){
                hands.emplace_back(hand,PokerHand::getStrength(hand));
            }
            sort(hands.begin(), hands.end(), [](auto a, auto b){
                return a.second < b.second;
            });
            cout << PokerHand::print(hands) << endl;
        }else if (g_ == "texas-holdem"){
            string d;
            iss >> d;
            vector<pair<string,int>> hands;
            string hand;
            while(iss >> hand){
                hands.emplace_back(hand, PokerHand::getBestStrength(hand, d));
            }
            sort(hands.begin(), hands.end(), [](auto a, auto b){
                return a.second < b.second;
            });
            cout << PokerHand::print(hands) << endl;
        }else{
            string d;
            iss >> d;
            vector<pair<string,int>> hands;
            string hand;
            while(iss >> hand){
                hands.emplace_back(hand, PokerHand::getBestOmaha(d,hand));
            }
            sort(hands.begin(), hands.end(), [](auto a, auto b){
                return a.second < b.second;
            });
            cout << PokerHand::print(hands) << endl;
        }
    }

}