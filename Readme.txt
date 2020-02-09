Lưu ý: chương trình này chạy trên Linux hoặc Ubuntu, bạn có thể chạy máy ảo hoặc cài đặt Linux (Unbuntu) với Windows.
	Bạn chạy bằng Visual Studio Code, bạn chỉ cần nhấp F5 và chạy chương trình bình thường trong cửa sổ Terminal.
	
I. Cách Build chương trình
	1. Bạn chạy bằng Visual Studio Code, bạn chỉ cần nhấp F5 và chạy chương trình bình thường trong cửa sổ Terminal.

	2. Hoặc kết hợp giữa VS Code với Terminal. Bạn nhấn Ctrl + Shirf + B để build chương trình.
	Sau đó bạn nhấn Ctrl + Alt + T  để mở Terminal lên.
	Cuối cùng bạn tìm file thực thi lúc nãy bạn vừa build (file này nằm trong thư mục Files) sau đó kéo thả nó vào terminal.
	Nhấn Enter để chạy chương trình.

	3. Cách cuối cùng là bạn chỉ dùng Terminal.Bạn mở Terminal lên ( Ctrl + Alt + T). 
	Gõ theo cú pháp biên dịch chương trình C như sau:
		$ gcc [tên file .c] -o [tên file thực thi] → sau đó nhấn Enter
		$./[tên file thực thi] →  sau đó nhấn Enter.
II. Các chức năng có thể thực hiện được
1. Thực thi một lệnh thông thường, ví dụ như
	+ ls
	+ ls -l
	+ cat [tên file]
	+ clear
	+...
2. Sử dụng hàm thông thường với “&”
	+ ls &
	+ ls -l &
	+ cat [tên file] &
	+...
3.Cung cấp lịch sử
-> Nhấn !! để thực thi lại lệnh trước đó
4.Chuyển hướng input
	+ wc < newfile (newfile đã được)
5.Chuyển hướng output
	+ echo 'hello' > newfile (newfile đã được)
6.Chuyển hướng input có dấu ‘&’
	+ wc < newfile &
7.Chuyển hướng output có dấu ‘&’
	+ echo 'I love OS' > newfile &
	+ cat newfile (để mở ra xem)
8.Sử dụng pipe()
	+ printf 'OS \n OS\n' |wc -l
	+ ls -l |tail -4
9.Sử dụng pipe() với dấu ‘&’
	+ printf 'OS \n OS\n' |wc -l &
	+ ls -l |tail -4 &
10.Thoát khỏi chương trình
	+exit