### What's this 
- [桃華み](http://bode-mmk.hatenablog.com/entry/2017/12/09/221315)の記事に関するリポジトリ
- 記事の下書きと、小町算を解くためのプログラムが入っている
```cpp
g++-8 -std=c++17 main.cpp -o momoka.out
```
- optional等使っているので必ずC++17環境でのビルドが必要だ
- 一番後ろに合わせたい答えを入力する それまでに使用する数字を指定する
```
./momoka.out <value1> <value2> <value3> ... <answer>
```
例えば、`1 2 3 4`を使って`10`を求めたい場合、
```
./momoka.out 1 2 3 4 10
```
などとしてやればよい
