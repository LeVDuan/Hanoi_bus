# Hanoi_bus
ứng dụng tìm cách đi xe buýt Hà Nội với dữ liệu chuyến xe và bến được lưu trong `hanoi-bus.txt`\
Với điều kiện: đi qua ít bến nhất và không giới hạn chuyển tuyến\
Ứng dụng được xây dựng bằng `ngôn ngữ C` và một số thư viện\
Hệ điều hành: `Ubuntu`
# Biên dịch chương trình
Chương trình sử dụng công cụ `Cmake` và `ninja` để biên dịch, bạn cần cài đặt `Cmake` và `ninja` để biên dịch\
khi đã đầy đủ công cụ 
khi đã đầy đủ công cụ hãy clone về máy bằng lênh:\ 
(Bạn phải cài đặt `git` trong thiết bị để thực thi lệnh duới đây)
```
git clone https://github.com/LeVDuan/Gtk_tutorial_C.git
```
sau đó hãy đi đến thư mục `b` :
```
cd Hanoi_bus/b
```
tại thư mục `b` gõ lệnh sau:
``` 
./ninja
./bus_main
```
# Thư Viện
* Thư viện sử dụng `Cgen` và `Cgraph` được xây dựng và phát triển bởi thầy [Nguyễn Bá Ngọc](https://github.com/bangoc) : 
[cgen](https://github.com/bangoc/cgen), [cgraph](https://github.com/bangoc/cgraph)
* Thư Viện libfdr của jimplank: [linkdownload](http://web.eecs.utk.edu/~jplank/plank/classes/cs360/360/notes/Libfdr/) hoặc tại [libfdr_github](https://github.com/bangoc/libfdr)
