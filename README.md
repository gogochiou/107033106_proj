# 107033106_proj
> 2021 Data Structure Final Project
## 工作日誌
- 12/20 
    > 1.設置環境
- 12/21
    > 1.確認可以讀檔 <br>
    > 2.解決 .h 檔案連結問題 <br>
- 12/22
    > 1.成功讀取檔案並初始化station (之前沒發現split string的方法，會漏掉最後一筆資料)
- 12/23
    >1.寫完station 和 bike的初始化程式
- 12/24
    >1.完成user的程式  <br>
    >2.完成normal的版本(還沒輸出status.txt)，有個bug尚未解決
    >>BUG : <br> 
    關掉calculateRevenue(userID, 0)後，再bike pop的部分，出現double free的error <br>
    若再把BikeList::pop()內的 delete temp註解掉，程式則會運行到一半卡住


## 學習日誌
**1. C++讀檔** <br>
>參考連結 <br>
>[一、seekg函數](http://c.biancheng.net/view/1541.html) <br>
>[二、c++ fastest way to read only last line](https://stackoverflow.com/questions/11876290/c-fastest-way-to-read-only-last-line-of-text-file) <br>
>[三、c++ read file twice](https://stackoverflow.com/questions/16393583/how-to-read-same-file-twice-in-a-row) <br>
>[四、c++ split string using delimiter](https://stackoverflow.com/questions/14265581/parse-split-a-string-in-c-using-string-delimiter-standard-c) <br>

**2. C++ double free detected in tcache 2** (尚未理解)
>參考連結 <br>
>[一、野指標](https://www.itread01.com/content/1548459559.html) <br>
>[二、double free](https://stackoverflow.com/questions/67359233/what-does-this-error-means-free-double-free-detected-in-tcache-2) <br>

&emsp;&emsp;大部分網路上的解法是說要使用 call by reference ? 但是我使用類似方法依然無效。後來發現有人說是因為delete刪除的是指標指向的記憶體空間，所以在delete後要把指標設為NULL，雖然蠻合理的，但不知道為啥我的程式在 **BikeList::pop()** 就是會卡住，而且理論上我的temp是區域變數，為何會出現double free的情況?另外我也嘗試過delete後設為NULL，狀況依然一樣。

&emsp;&emsp;最大的問題是，如果我今天把calculateRevenue這個函式打開，一切就可以正常運作，但是那函式根本不會動到物件裡的東西，只有從中return數值回來...。而且我在 **BikeList::push()** 裡面也有使用delete，就不會有這個問題。

&emsp;&emsp;後來發現是我在 **BikeList::push()** 的時候，不小心delete了temp這個指標，也就是說會把我temp指向的記憶體位置清空，所以我實際上first指向的東西會是一個NULL的空間，改掉此問題就解決惹，但還沒想清楚實際的影響在哪裡?!

```cpp
/* BikeList::push() */
if(id < first->ID){
    Bike* new_bike = new Bike(station_num, bike_num, first);
    first = new_bike;
    // delete new_bike;  /* this is wrong */
    new_bike = NULL;
    return;
}
```




