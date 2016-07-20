#include <a_samp>
#include "observe.inc"

stock MyCb(prev, curr) {
	printf("Change from %d to %d", prev, curr);
}

new var1, var2, var3, var4;

observer Var1(p, c) {
	var2 = c + 1000;
}

observer Var2(p, c) {
	var3 = c * 2;
}

observer Var3(p, c) {
	var4 = -c;
	printf("var3a %d -> %d", p, c);
}

observer Var3b(p, c) {
	printf("var3b %d -> %d", p, c);
}

observer Var3c(p, c) {
	printf("var3c %d -> %d", p, c);
}

observer Var3d(p, c) {
	printf("var3d %d -> %d", p, c);
}


new array_sum = 0;

observer ArrayChange(p, c, idx) {
	printf("array[%d] %d -> %d", idx, p, c);
	array_sum += c - p;
}

observer ArrayChange2(p, c, idx) {
	printf("array2[%d] %d -> %d", idx, p, c);
}

observer ArrayChange3(p, c, idx) {
	printf("array3[%d] %d -> %d", idx, p, c);
}

observer FalsePositive(p, c, idx) {
	printf("false_positive[%d] %d -> %d", idx, p, c);
}

new test = 11;
new array[10];
new other[5];
new false_positive[3] = {11, 22, 33};

main() {
	new a = 11, b = 22, c = 33;
	new d = ObserverTest();

	assert a == 11;
	assert b == 22;
	assert c == 33;
	assert d == 44;
}

public InstallObservers() {
	ObserveVar(test, MyCb);
	ObserveVar(var1, Var1);
	ObserveVar(var2, Var2);
	ObserveVar(var3, Var3);
	ObserveVar(var3, Var3b);
	ObserveVar(var3, Var3c);
	ObserveVar(var3, Var3d);

	ObserveArray(false_positive, FalsePositive);
	ObserveArray(array, ArrayChange);
	ObserveArray(array, ArrayChange2);
	ObserveArray(array, ArrayChange3);
}

ObserverTest() {
	new a = 111, b = 222, c = 333;
	new s = 5, q, w, e;
	a += (a+q+w+e)*0;

	new sum = 0;
	for (new i = 0; i < 10; i++) {
		new v = i * 10;
		array[i] = v;
		sum += v;
	}

	printf("array_sum = %d", array_sum);
	printf("sum should be %d", sum);

	// array[5] = 3333333;
	// printf("array[5] is %d", array[5]);
	// array[5] = 5;
	// printf("array[5] is %d", array[5]);

	var1 = 55;
	printf("var1 = %d", var1);
	printf("var2 = %d", var2);
	printf("var3 = %d", var3);
	printf("var4 = %d", var4, other);

	test = 5;
	test = 6;
	test += 10;
	#emit CONST.alt 1234
	test++;
	#emit STOR.alt test
	test = test / 2;

	for (test = 0; test < 10; test++) {}

	printf("done. test is %d", test);

	#emit CONST.pri false_positive
	#emit CONST.pri 0
	#emit CONST.alt 0
	#emit STOR.I

	printf("falst_positive = %d, %d, %d", false_positive[0], false_positive[1], false_positive[2]);

	assert a == 111;
	assert b == 222;
	assert c == 333;

	return 44;
}
