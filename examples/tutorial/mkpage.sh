#! /bin/bash
std_example() {
echo "==== $1 ===="
echo "'''Source code'''<br/>"
echo "<pre>"
cat $2.pi
echo "</pre>"
echo "'''Output'''<br/>"
echo "<pre>"
echo ">./$2"
timeout 1 ./$2
sleep 1
echo "</pre>"
}
inp_example() {
echo "==== $1 ===="
echo "'''Source code'''<br/>"
echo "<pre>"
cat $2.pi
echo "</pre>"
echo "'''Output'''<br/>"
echo "<pre>"
echo ">echo $3 | ./$2"
echo $3 | timeout 1 ./$2
sleep 1
echo "</pre>"
}
arg_example() {
echo "==== $1 ===="
echo "'''Source code'''<br/>"
echo "<pre>"
cat $2.pi
echo "</pre>"
echo "'''Output'''<br/>"
echo "<pre>"
echo ">./$2 $3"
timeout 1 ./$2 $3
sleep 1
echo "</pre>"
}
slw_example() {
echo "==== $1 ===="
echo "'''Source code'''<br/>"
echo "<pre>"
cat $2.pi
echo "</pre>"
echo "'''Output'''<br/>"
echo "<pre>"
echo ">./$2"
timeout $3 ./$2
sleep 1
echo "</pre>"
}
echo "== Tutorial =="
echo "=== 1 Basics ==="
std_example "1.1 Hello World" 1.1-hello_world
std_example "1.2 Fork" 1.2-fork
std_example "1.3 Methods" 1.3-methods
echo "=== 2 Communication ==="
std_example "2.1 Message" 2.1-message
std_example "2.2 Service" 2.2-service
std_example "2.3 Branch" 2.3-branch
std_example "2.4 Recursion" 2.4-recursion
std_example "2.5 Multiparty" 2.5-multiparty
echo "=== 3 Standard libraries ==="
arg_example "3.1 Args" 3.1-args arg
slw_example "3.2 Timer" 3.2-timer 6
std_example "3.3 File" 3.3-file
std_example "3.4 Convert" 3.4-convert
echo "=== 4 Purity ==="
std_example "4.1 Purity" 4.1-purity
inp_example "4.2 Fib" 4.2-fib 30
inp_example "4.3 Fib Time" 4.3-fib_time 30
inp_example "4.4 Fact" 4.4-fact 30
echo "=== 5 Extras ==="
inp_example "5.1 Assertions" 5.1-assertions true
std_example "5.2 Host" 5.2-host
