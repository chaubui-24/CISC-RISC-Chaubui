Bùi Phúc Châu

So sánh kiến trúc CISC và RISC

1.  Khái niệm cơ bản về CISC và RISC

```{=html}
<!-- -->
```
a)  CISC (Complex Instruction Set Computer)

    -   Máy tính tập lệnh phức tạp (CISC) là một kiến trúc vi xử lý sử
        dụng các lệnh phức tạp để cải thiện hiệu suất.

    -   Sử dụng nhiều thao tác phức tạp trong một lệnh, giảm số lượng
        lệnh cần thiết cho mỗi tác vụ và do đó giảm lượng mã cần thiết,
        nhưng đồng thời cũng sử dụng nhiều hơn một chu kỳ xung nhịp để
        thực thi một lệnh.

b)  RISC (Reduce Instruction Set Computer)

    -   Máy tính tập lệnh rút gọn (RISC) là một kiến trúc vi xử lý hướng
        đến sự đơn giản và tốc độ, nhằm tối ưu hóa việc thực thi lệnh
        bằng cách sử dụng một tập lệnh nhỏ hơn, trực quan hơn.

    -   Cho phép thực thi lệnh nhanh hơn và tạo điều kiện thuận lợi cho
        kỹ thuật pipeline -- kỹ thuật mà trong đó nhiều lệnh được chồng
        chéo lên nhau trong quá trình thực thi.

```{=html}
<!-- -->
```
2.  **Ưu điểm, nhược điểm của CISC và RISC**

+-------+-------------------------------+------------------------------+
|       | Ưu điểm                       | Nhược điểm                   |
+=======+===============================+==============================+
| CISC  | \- Tập lệnh "mạnh", dòng lệnh | \- Câu lệnh phức tạp, gây    |
|       | ít.                           | khó khăn trong thiết kế và   |
|       |                               | sản xuất.                    |
|       | \- Chiếm ít bộ nhớ.           |                              |
|       |                               | \- Thực thi mỗi lệnh tốn     |
|       | \- Xử lý được các dữ liệu     | nhiều thời gian, từ đó lượng |
|       | phức tạp.                     | tiêu thụ điện năng cao.      |
|       |                               |                              |
|       | \- Phổ biến, chiếm lượng      |                              |
|       | người dùng lớn.               |                              |
|       |                               |                              |
|       | \- Xử lí bộ nhớ trực tiếp.    |                              |
+-------+-------------------------------+------------------------------+
| RISC  | \- Câu lệnh đơn giản, xử lý   | \- Nhiều lệnh để xử lý một   |
|       | dữ liệu nhanh hơn.            | tác vụ, chiếm nhiều bộ nhớ.  |
|       |                               |                              |
|       | \- Tiêu thụ điện năng ít hơn. | \- Cần phụ thuộc vào trình   |
|       |                               | biên dịch phức tạp.          |
|       | \- Thuận lợi trong sản xuất.  |                              |
|       |                               | \- Chỉ có thể truy cập bộ    |
|       |                               | nhớ qua 1 vài câu lệnh       |
|       |                               | (load, store,...).           |
+-------+-------------------------------+------------------------------+

3.  **So sánh CISC và RISC**

+-------------------+--------------------------+-----------------------+
|                   | CISC                     | RISC                  |
+===================+==========================+=======================+
| a.  Cấu trúc tập  | \- Câu lệnh phức tạp, có | \- Câu lệnh đơn giản, |
|     lệnh          | thể xử lý một tác vụ với | một tác vụ cần nhiều  |
|                   | ít câu lệnh.             | câu lệnh.             |
+-------------------+--------------------------+-----------------------+
| b.  Tốc độ xử lý  | \- Chậm                  | \- Nhanh              |
+-------------------+--------------------------+-----------------------+
| c.  Kích thước    | \- Ít câu lệnh nên chiếm | \- Chiếm nhiều dung   |
|     chương trình  | ít dung lượng.           | lượng do có nhiều câu |
|                   |                          | lệnh.                 |
+-------------------+--------------------------+-----------------------+
| d.  Độ phức tạp   | \- Cao để xử lý được câu | \- Thấp vì chỉ cần xử |
|     phần cứng     | lệnh phức tạp.           | lý những câu lệnh đơn |
|                   |                          | giản.                 |
+-------------------+--------------------------+-----------------------+
| e.  Ứng dụng thực | \- CPU x86 -- thường có  | \- ARM -- thường có   |
|     tế            | trong máy tính cá nhân.  | trong smartphone,     |
|                   |                          | tablet.               |
|                   | \- Máy chủ và máy trạm.  |                       |
|                   |                          | \- Hệ thống nhúng vi  |
|                   |                          | điều khiển trong oto, |
|                   |                          | tủ lạnh, TV,...       |
+-------------------+--------------------------+-----------------------+

4.  **Quan điểm cá nhân**

\- Trong hệ thống nhúng hiện nay, cá nhân mình thấy kiến trúc RISC phù
hợp hơn. Vì các thiết bị có vi điều khiển hiện nay (điện thoại, oto,...)
đều tránh việc sử dụng điện năng cao, cần thiết kế gọn nhẹ, tốc độ xử lý
cao,... Những yếu tố trên RISC đều đáp ứng rất tốt. Từ đó xu hướng sử
dụng RISC sẽ phát triển tốt trong tương lai.
