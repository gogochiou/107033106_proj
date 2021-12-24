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



