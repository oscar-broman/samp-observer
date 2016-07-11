# samp-observer

Easily observe variables for changes. At the moment the variable is modified, the callback will be invoked.

There is no need for custom syntax, and this include does not use timers or loops to check for changes!

## Example usage

```SourcePawn
new g_Test = 1234;
new g_TestString[32];
new g_Array[5];
new g_ArraySum = 0;

main() {
	// Now OnTestChange will be executed
	g_Test = 789;

	print(g_TestString); // Test is 789

	// Every time this happens, OnArrayChange will be executed
	for (new i = 0; i < sizeof(g_Array); i++)
		g_Array[i] = (i + 1) * 10;

	// g_ArraySum will be automatically updated in OnArrayChange
	printf("The sum of g_Array is %d", g_ArraySum);

	for (new i = 0; i < sizeof(g_Array); i++)
		g_Array[i] *= 10;

	printf("The sum of g_Array is %d", g_ArraySum);
}

// Executed every time g_Test is changed
observer OnTestChange(previous, current) {
    printf("Test changed from %d to %d", previous, current);

	// Update a string when this value changes
	format(g_TestString, sizeof(g_TestString), "Test is %d", current);
}

// Executed every time g_Array is changed
observer OnArrayChange(prev, curr, idx) {
	printf("g_Array[%d] %d -> %d", idx, prev, curr);

	// Keep track of the array's sum without ever looping through it
	g_ArraySum += curr - prev;
}

public InstallObservers() {
	ObserveVar(g_Test, OnTestChange);
	ObserveArray(g_Array, OnArrayChange);
}

```

## Usage and requirements

You need the [amx_assembly](http://github.com/zeex/amx_assembly) lib. Simply place it with your include files, along with samp-observer, then include it like this:

```SourcePawn
#include <samp-observer/observe>
```

## How it's done

The include will modify the AMX code to add the function call every time an observed var/array is modified.

When you change an observed variable, the observer callback will be executed before the code continues.

## Compatibility

This include is compatible with and without both crashdetect and the JIT plugin.

Credits

This would not be possible without Zeex's amx_assembly lib and Y-Less's contributions to it. Most importantly, the latest [codescanner](https://github.com/Zeex/amx_assembly/blob/master/codescan.inc) addition.

## Limitations

You can only observe global variables and 1-dimensional arrays. I hope to bring 2-dimensional arrays to this include also!
