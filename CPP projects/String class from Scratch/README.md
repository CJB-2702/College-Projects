# ECE141a-WI21-Assignment1 
#### Due Jan 22, 2021 @ 11:30pm (PST)

## Let's build a custom string class

In our recent video lecture, "Strings Rock", we discussed how strings represent a microcosm of the many of the challenges found in computer science. Complexity, performance, security, resource management, and most especticially - software design challenges are all real concerns when building a string class. 

In this assignment, you'll build a simplified string class, from a prescribed class interface that we define below. In order to your class implementation, we will compare the operations that your class provides against the implementation found in `std::string`. 

> NOTE: You may not use any part of `std::string` in the implementation of your string class. However, you many use any C-library functions (e.g. strlen) as well as functionality provided by the STL (again, except for std::string).

## Deliverables

You will implement a class called `String`, and a delegated buffer management class call `BufferManager`.  Save your string class to the files `String.hpp` and `String.cpp`.  You can use any file names you want for your `BufferManager` class. You may create as many other support classes as you like. 

> Note: You may implement the `BufferManager` class any way you like, but it **MUST** serve as a delegate to the `String` class to manage the underlying character buffer.  You can make the `BufferManager` a base class for your `String`, include a `BufferManager` as a data member of your `String`, or any other approach you deem reasonable. Furthermore, the interface to your `BufferManager` class is up to you -- so long as the class effectively manages your internal string data buffer. 

Below is the interface that your class must provide. NOTE: You'll notice that many methods have two-versions: one that accepts a `String` object, and one that accepts a `const char*`. If you're thoughtful about your implementation, you can write one of these methods in terms of the other, and therefore reduce your workload considerably.  Also note that we modeled our `String` class on the `std::string` class to help make testing and validation easier for you.

```
class String {
    String(const String &aString, size_t aPresize=0);
    String(const char* aBuffer, size_t aPresize=0);
    ~String();
    String& operator=(const String &aCopy);
    String& operator=(const char* aBuffer);
    
    //accessors...
    size_t size() const;    

    //mutation methods...
    char&       operator[](int pos);
  
    String  operator+(const String &aString);  //append (construct) methods
    String  operator+(const char* aBuffer);
    
    String& operator+=(const String &aString);  //append (to self) methods
    String& operator+=(const char* aBuffer);

    String& insert(size_t anIndex, const String &aString, size_t aMaxCopyLen);
    String& insert(size_t anIndex, const char* aString, size_t aMaxCopyLen);
      
    String& replace(size_t anIndex, size_t aMaxCopyLen, const String &aSubString);
    String& replace(size_t anIndex, size_t aMaxCopyLen, const char *aSubString);
        
    String& erase(size_t anIndex, size_t aCount); //from given position, remove N chars...("cut")
    
      //Comparision methods...
    int compare( const String& aString ) const; //[-1 if <, 0 if ==, 1 if >]
    
    bool operator==(const String &aString) const;
    bool operator==(const char* aBuffer) const;

    bool operator<(const String &aString) const;
    bool operator<(const char* aBuffer) const;

      //Search...
    int find( const String &aSubString, size_t anOffset = 0 ) const;
              
      //stream...
    friend std::ostream& operator << (std::ostream &out, const String &aStr);
    friend std::istream& operator >> (std::istream &in,  String &aString);      
}
```

## String Method Details (Implementation details for key methods listed above):

### OCF
We're not repeating them here, but it's expected that your class will properly implement the OCF methods. 

### `String& operator=(const char* aBuffer);`
Your `String` class must provide an assignment operator that accepts a `const char*`. 
  
### `size_t size()`; 
The `size()` method returns the current length of the underlying string.

### `char& operator[](size_t anIndex);` 
This method permits the caller to access a character at a given index. This method must guard against out-of-bounds accesses (0<=index<length). Should an out-of-bounds error occur your `String` must throw a `std::out_of_range` exception.

### `String  operator+(const String &aString);`
This method creates and returns a new string, equivalent to the given `aString` arugment appended to the string object in scope.

### `String  operator+(const char* aBuffer);`
Same as above, but accepts a `const char*` for backward compatiability.

### `String& operator+=(const String &aString);`
This overload operator (`+=`) appends the given `aString` argument to your existing string.  Return a reference to `*this`. 

### `String& operator+=(const char* aBuffer);`
Same as above, but accepts a `const char*` for backward compatiability.
         
### `String& insert(size_t anIndex, const String &aString, size_t aMaxCopyLen)`
This methods allows a caller to insert the given `aString` argument into the current string at a given `anIndex`. The 3rd argument (`anInsertLimit`) allows the caller to limit the number of characters from the given `aString` argument that are inserted in the string in scope. For example, if given `aString` said "Hello there", and I only want to insert "Hello", I would pass 5 as the last argument. If the last argument is 0, it means to insert all the characters from the given string.

### ` String& insert(size_t anIndex, const char* aString, size_t aMaxCopyLen);`
Same as above, but accepts a `const char*` for backward compatiability.
   
### `String& replace(size_t anIndex, size_t aMaxCopyLen, const String &aSubString);`
This method replaces a portion of the current string object with the contents given in `aSubString`, starting at anIndex.  The `aMaxCopyLen` allows the caller to pass a longer string (as a replacement) but only copy a portion of that `aSubString` into the current string. If the caller passes a `aMaxCopyLen` as "0", then the entire contents of `aSubString` will be inserted as the replacement in the current string.

### `String& replace(size_t anIndex, size_t aMaxCopyLen, const char *aBuffer);`
Same as above, but accepts a `const char*` for backward compatiability.

### `String& erase(size_t anIndex, size_t aCount);`
This method causes `aCount` characters in the underlying string to be removed starting at `anIndex`. It is not considered an error if the `aCount` size extends past the end of the underlying string. With right arguments, this method can be used to remove characters from the middle of the string, or to truncate the string, which discards all characters after a certain point.   
 
### `int compare( const String& str ) const;`
This method compares the given `aString` against the string in scope. We use the familiar results (from C) to indicate how the cast-sensitive strings compare lexographically. Return -1 if `*this` string is less than the given string. Return 0 if they're equal, and return 1 if `*this*` string is greater than the given `aString`.  
  
### `bool operator==(const String &aString) const;`
This Method return `true` `*this` this is equal to the given `aString`. 

### `bool operator==(const char* aBuffer) const;`
Same as above, but accepts a `const char*` for backward compatiability.

//NOTE: Follow a similar pattern for operator<()...

### `int find( const String &aSubString, size_type anOffset = 0 ) const;`
This method attempts to local the given `aSubString` within `*this` string, starting a given `anOffset`, searching left to right. If found, return the index associated with the index of the first matching chararater.  Return -1 if no match it found.

## BufferManager Class Details

Your `BufferManager` class is a delegate class that will actually manage the data buffer that holds your string data. You are free to implement this class any way you like. Note, however, that this class MUST perform operations related to creating, resizing, and (ultimately) destroying your data buffer.  In other words, the `BufferManager` class is the interface your string class will use to do memory management related operations. Your string class can (and probably should) be responsible for managaing the actual character data (such as when you insert, append, or change the associated string).  

## Testing

This assigment is autograded by "Vlad-the-compiler", our auto-grading robot who has a bad haircut and an even worse attitude. It's best to try to stay on Vlad's good side by turning your homework in on time. Vlad provided an auto-grading template on github classroom. You can see the result in the `main.cpp` file of this assignment. There are four tests that Vlad will run:  "compile", "OCF", "mutate', and "compare". Each is run independently, and each is worth individual points. 

Vlad will run all four tests using the auto-grader mechanism. You can run any test you want individually by passing a command-line argument to your running application. This command line argument is picked up by your main file, and used to determine which test to run *locally*. 

### Grading  Rubric 

- Your code compiles : 20%
- Your string class OCF methods: 20%
- Mutations methods (append, insert, replace, erase...): 40%
- Comparision functions: 15%
- Coding style: 5%  (well structured, readable code with clear identifiers)

## Submitting your work - Due Jan 22, 2021 @ 11:30pm (PST)

You turn in your assignment by checking your code back into github. Each you do this, Vlad will try to retest your code.  You may check-in your code as often as you like without penalty before the assignment due date. 

Inside the project directory you'll find a file called `student.json`. This file contains a few fields that identify you, and provide helpful information to "Vlad-the-Compiler" -- our auto-grading robot.  Please make sure to fill out the personal information in student.json file. Don't forget to give us an estimate for how many hours it tool you to complete this assignment.

Good luck!


