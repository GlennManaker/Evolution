# TEXAS, OMAHA, FIVE-CARD-DRAW
The main idea is to give each hand an equivalent in number.  
Let's represent a number in binary notation  
We define the class to which the hand belongs and put 1 in << (28 - (combination number)) (<< bit shift)  
Sort by relevance card, allot to the class, and then sort by the number of the same in the dec
~~~c++
for (int i= temp.size() - 1; i>=0; --i){
    result+=(pow(13,i)*score[temp[i].first]);
}
~~~  
If the classes are the same compare the strength in the class  
We get cards by enumerating all combinations - a drawback that increases the constant in complexity
# RUN (C++)
Just compile cpp file. STDIN, STDOUT
