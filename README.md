# Small Args ![travisimg](https://travis-ci.org/Rookfighter/small-args.svg?branch=master)

Small Args is a lightweight header-only crossplatform argument parser for the C / C++ programming languages.
The library focuses on simplicity and high compatibility to various platforms and compilers.
It implements Posix style argument parsing, but is not compliant to the standard.

Small Args' parsing capabilities are kept minimal and focus on daily and most important features.
If you feel like any important feature is missing, please contact me or implement it yourself and submit a pull request.

## Install

Simply copy the header files into your project and include them in your source code and you're good to go. 

## Usage

### C

For using Small Args include the C header file. 

```C
#include <smallargs.h>
```

Options that should be parsed by your application have to be specified as an array of ```sarg_opt```.
Each option requires either a short or a long name and a type. An optional help text and a callback function can be specified if needed.
The array is NULL terminated. The NULL element that terminates the array has no long and no short name.

```C
sarg_opt my_opts[] = {
    {"h", "help", "show help text", BOOL, NULL},
    {NULL, NULL, NULL, INT, NULL}
};
```

Available types for options in Small Args are:

* ```int```
* ```unsigend int```
* ```double```
* ```bool```
* ```string```

The types ```int``` and ```unsigend int``` accept decimal, hexadecimal (leading ```0x```) and octal number (leading ```0```) formats.

After initialising a ```sarg_root``` with your option list, use ```sarg_parse``` to parse an argument vector.

```C
int main(const int argc, const char **argv)
{
	sarg_root root;
	int ret;

	ret = sarg_init(&root, my_opts);
	// handle error ...

	ret = sarg_parse(&root, argv, argc);
	// handle error ...

	// ...
}
```

You can also read arguments from a file. The file should contain one option per line and if the
option requires another parameter seperate it with a white space. Use ```sarg_parse_file``` to read
form a file.

```C
int main(const int argc, const char **argv)
{
	// ...

	ret = sarg_parse_file(&root, "path/to/file");
	// handle error ...

	// ...
}
``` 

If parsing has succeeded the results can be accessed using ```sarg_get```. You can either use the short or the long name of an option.

```C
int main(const int argc, const char **argv)
{
	// parse your stuff here ...

	sarg_result res;
	ret = sarg_get(&root, "help", &res);
	// handle error ...

	if(res->bool_val)
	{
		sarg_help_print(&root);
		return 0;
	}

	// ...
}
```

Also never forget to clean things up ;).

```C
int main(const int argc, const char **argv)
{
	// ...

	sarg_destroy(&root);

	return 0;
}
```
	
For more information on how to use callbacks and fully working examples have a look at the ```sample/``` directory.

### C++

**TBD. The C++ wrapper is not yet finished!**

If you want to use the C++ interface include the .hpp header file.

```
#include <smallargs.hpp>
```
