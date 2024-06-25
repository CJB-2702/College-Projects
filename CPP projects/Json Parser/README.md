# wi21-assignment3
### Due Feb 20, 2021 -- at 11:30pm (PST)

## Introduction

In this assignment, you're going to implement "object persistence" using JSON. Object persistence refers to saving and loading our C++ objects from a persistent storage system (like your hard-drive, or the cloud). In this case, we're going to use a stream. 

Before we begin, take care to read through this assignment a few times. At first, it may seem like a lot. In reality, we provide you lots of working starter code. You'll write a few hundred lines of code, that can do the following things:

1. Teach your in-memory C++ objects how write/encode themselves into a text stream in JSON format.
2. Read a JSON text stream, and construct an _intermediate internal respresentation_ (`JSONModel`). 
3. Re-construct your original C++ objects from the `JSONModel`, using a `Builder` as a multi-object factory.

This process may sound familiar to you. It's essentially the same thing the Web does when we view a website. Or read a Microsoft Word document.  Or play a game, or listen to music, or move data around a network. You'll discover that learning to encode, decode, and reconstruct your data as it moves around a network is an extremely common occurrance, and a useful skill to have. Moreover, we'll use these skills in ECE141b.

## What is JSON?

JSON or JavaScript Object Notation is a lightweight text-based open standard designed for human-readable data interchange. It is easy to read and write, is language independent, and most importantly, is it platform independent. You can create a JSON file in any language on any platform, and easily send it to any other platform (or language) and use that data.  

JSON was designed and used originally to transmit javascript data between a server and web applications. Web services and APIs use JSON format to provide public data. It is also used for serializing and transmitting structured data over network connection. As JSON has become more popular, it has become a nearly standard means for encoding and sharing typed information in most platforms. 

#### Other JSON Resources
There are a lot of good resources on the web if you want to read further. Sharing a few here -

1. https://www.tutorialspoint.com/json/index.htm
2. https://json-schema.org/understanding-json-schema/

### JSON Syntax
At the top level, JSON data can either be an Object or a **collection**. Curly braces `{}` hold objects and Square brackets `[]` hold collections of data or objects.

In an object, data is represented as set of key-value pairs. Each key is followed by colon `:` followed by a value. Individual key-value pairs are delimited by a comma `,`. For example - `"courseId" : "ECE141a"` is a key-value pair. A JSON object can contain any number of key-value pairs, that represent any type of information your care to encode.  Notice that the key is always a string surrounded quotes `""`. For example - courseId, courseName, studentList, gpa etc. are all keys in Example #1 shown below.

JSON supports multiple data types for values. Value can be one of the following types (see example JSON for examples) -

**number** - floating point or integer constants. For instance - value for the field gpa

**string** - a Unicode string in double quotes(""). For instance - value for fields courseId, courseName etc.

**boolean** - boolean constant (true/false). For instance - value for the isGradStudent field in the student object.

**object** - Value can also be another JSON object. For instance - value for the schedule key. This gives JSON a tree like structure and makes the storage of hierarchial data easy.

**list** - The value can also be a list of items. Square brackets `[]` are used to hold values. Values within the list are separated by commas. The value can be any valid value type(number, string, boolean, JSON object or even another list). For instance - the value for the key studentList is an array of JSON objects.

**null** - The value for a particular key can also be null. This is equivalent to key being not present.

The flexibility in the value types allows us to store complex data structures. For example - You can have an object which contains a list of objects where each object has a list of lists.

Notice that there are no double quotes "" for primitive values like number, boolean and null. true and "true" are different values.  
Here's a random JSON example:

#### **EXAMPLE #1**
```
{
    "courseId":"ECE-141A",
    "courseName":"Software Foundations - I",
    "studentList":[
        {
            "id":"foo",
            "name":"Foo Bar",
            "pId":"A22222222",
            "gpa":3.95,
            "isGradStudent":true
        },
        {
            "id":"bar",
            "name":"Bar Foo",
            "pId":"A99999999",
            "gpa":3.85,
            "isGradStudent":false
        }
    ],
    "schedule": {
      "lectures": "Tu-Thu 9:30AM",
      "discussions": "Mon 4:00PM"
    }
}
``` 

#### How to Make Sense of JSON

If you look at the JSON example above, you'll notice that there are common patterns in the struture of JSON documents. So common, in fact, that we can rely on these patterns, and describe them as an _intermediate internal respresentation_, using basic C++ classes. As it turns out, everything in JSON is either a `JSONObject`, a `JSONList`, or a `JSONElement`. We say that `JSONObject` and `JSONList` are containers, because they can recursively contain other things.  A `JSONElement` is not a container.  Let's take a closer look.

#### `JSONElements` (key:value)

A `JSONElement` is represented by a key/value pair.  The `KEY` property is always a "quoted" string. The `value` property may be:
1. A constant (e.g. 123, 3.14, true)
2. A `JSONObject` {...}  //see below
3. A `JSONList` [...]  //see below

#### `JSONObjects`  

A `JSONObject` always begins and ends with curly braces `{...}`; it is usually preceded by a `KEY` property. A `JSONObject` may contain any number of `JSONElements` (key/value pairs).

Consider our example below. The outer `JSONObject` is anonymous (no given key). It contains two nested `JSONElements`, "thisthing" and "thatthing".  The _value_ of "thisthing" is a `JSONObject`, whereas the value of "thatthing" is a boolean constant. 

```
{
  "thisthing" : {"key1": "value1"},
  "thatthing" : true
}
```

#### `JSONLists` 

A `JSONList` always begins/ends with square braces `[...]`; it may (or may not) be preceded by a key. 

Inside the braces, a `JSONList` can contain an arbitrary list of constants, or a list of `JSONObjects`, separated by commas. 

```
"this_list" : [100,200,300],
"that_list" : [
  {"key1": "value1"},
  {"key1": "value1"}
]
```

To help you with your _intermediate internal representation_, we have already declared classes for `JSONElement`, `JSONList` and `JSONObject` for you to implement.

> **NOTE:** Every JSON document has only one parent element that wraps all the other stuff -- either a `JSONObject` or it's a `JSONList`.  In the example above, we used a `JSONObject` because the document is contained by curly braces `{...more json...}`.  The only other alternative we have as a top-level container is to wrap our JSON in a `JSONList`, using square brackets, e.g. `[10,20,30]`.  


## Writing Objects as JSON
The idea behind _persisting_ an object in JSON, is to allow an object in our C++ program to be stored on disk (or the cloud). When you restart your program, persisted objects can be reloaded from disk, and reconstructed as live objects in our running system. Imagine that you wrote a word processor in C++. A document might be comprised of a series of paragraph objects. When the user chooses the "save command" -- your document wouuld save each paragraph in JSON format inside a JSON file.  Later when the user wants to continue editing that document, the document is re-loaded from storage (as JSON), and new paragraph objects are constructed from the saved JSON data. 

> When you encode your object data properties into JSON, **MAKE SURE** that the fields are written out in the same order they are declared in the class. JSON doesn't care about the order, but one of our automated tests requires this. 


### Storable Example
Imagine we have two C++ classes (`Foo` and `Bar`). 

#### **EXAMPLE #2**
```
class Foo  {
    std::string name;
    bool        state;
};

class Bar  {
    int         size;
    std::string name;
    Foo         foo;
};
```
Imagine that we have a `Bar` object on the stack that we want to save as JSON. The following example shows the JSON we generate.  Since this is an object, it gets two sections: `meta` that describes the object (class name and version), and `members` that describes the data members. Notice that JSON uses commas to separate elements. 

#### **EXAMPLE #3**
```
{
    "meta"  : {
        "class": "Bar",
        "version" : 1.0
    },
    "members" : {
        "size"  : 100,
        "name"  : "bar-object",
        "foo": { 
            "meta"  : {
                "class": "Foo",
                "version" : 1.0
            },    
           "members" : {
              "name"  : "my foo",            
              "state" : true
           }
        }
    }
}
```

Our `Bar` class in the JSON shown above has three fields. Notice that intrinsic-type data members (`size` and `name`) are encoded using a simple ("Key" : "Value") format:

```
"size" : 100,
"name" : "bar-object"
```

However, the `foo` member is an object type, and gets encoded **as a JSON object**, wrapped in curly braces, with its own `meta` and `members` sections:
```
"foo": { 
    "meta"  : {
        "class": "Foo",
        "version" : 1.0
    },    
   "members" : {
      "name"  : "my foo",            
      "state" : true
   }
}
```

## Reading Data From JSON (Parsing and Decoding)
Once we have a JSON file, we need a way to open, read, and make sense of the contents of the file. We use the word "parsing" to refer to the process of reading data (text or binary), and extracting meaningful information. We have provided you with a working `JSONParser` class -- which can read the characters in a JSON file, and combine them into meaningful tokens ((words, numbers, punctuation, etc.). 

Your next step is to create an _intermediate internal respresentation_ that fully represents our JSON as "intermediate C++ objects". We declared a few classes for you (`JSONModel`, `JSONElement`, `JSONObject`, `JSONList`) for this purpose. The `JSONModel` class is the "root" object in this hierarchy, and it manages a collection of the other classes we just mentioned. 

As the `JSONParser` outputs a token sequence, your code will identify patterns. Once identified, you'll add elements to out your `JSONModel` hierarchy with elements that correspond to the JSON tokens you've just seen. This hierarchical `JSONModel` is a collection of `JSONObject`, `JSONList`, and `JSONElement` objects. You can implement your model any way you want -- but we suggest using the STL to save time and effort. 

If the JSON was well formed (correct), your internal `JSONModel` will be an perfect internal representation of the JSON file.  We can then move on to the next step of of re-constructing our class objects from your `JSONModel` using our builder pattern.

Now...take a moment to reflect on this. The parser generates tokens, which you interpret, to build out your `JSONModel` so that it's a reflection of the JSON text file we just read. Once complete, our `JSONModel` becomes input for our `Builder`, a meta-factory that re-constructs our C++ objects.

## Reconstructing Objects From JSON using the Builder

Our `Builder` class needs a valid `JSONModel` to work from. The `JSONModel` fully describes our C++ objects, including the values of every data member.  In this step, we're going to re-construct our `TestDocument` and its associated `TestClasses` (children) using the data found in our `JSONModel`. You'll write code that iterates the `JSONModel` object hierarchy. As you encounter each `JSONObject` in your `JSONModel`, you'll make the corresponding C++ Test class. The `JSONObject` in your `JSONModel` will contain the values of all the fields in your actual C++ Test classes. Use these values to properly construct each Test class.

The reconstruction processes continues this way, making each of the C++ objects you see as you iterate the `JSONModel` hierarchy. At some point, you'll have made all the C++ Test classes, and you can then fully construct your `TestDocument` object. That object is returned to the caller (in our testing system), and compared to the original `TestDocument` we created manually in C++.

---

## Challenges For This Assignment
In this assignment, you'll get experience generating, parsing/reading, and tokenizing JSON. You'll get experienced with building a hierarchical respresentational data-model, and you'll use a `Builder` to re-construct your C++ Test class objects.

### Challenge #1  - Implementing Your `JSONReader` class to decode a basic JSON stream

In this first challenge you'll implement your `JSONReader` class. It will use the `JSONParser` we provide to consume JSON data, and build an internal `JSONModel` that represents the JSON data you just read.  

#### STEP 1A -- Implement the `JSONReader` class
Implement `JSONReader` class so it can read and build an internal `JSONModel` of a given JSON file. As we discussed in class, this is a linear sequence of tokens. You'll want to save them on a stack, and re-evaluate the stack each time a new token arrives. Eventually, your stack will contain a complete JSON "fragment", such as a key:value pair. Now you're ready to convert these tokens into an object that can be stored in your `JSONModel` -- which is step #2 below.

#### STEP 1B -- Finalize the implmentation of `JSONModel`, `JSONList`, `JSONObject` and `JSONElement`
We provided starter declarations for each of these, but you'll need to finish them. 

Examine some JSON examples (above), and consider what types of data you might need to store.  For example, in your `JSONElement`, you'll need data to hold a key (string) and a value of various types.  Your `JSONObject` can hold a collection of `JSONElements` (as described in the sections above). None of these classes are complicated. Most only need one or two data members. and a corresponding constructor.

#### STEP 1C -- Implement  `JSONModel.debugDump()` method 
In this step, you're going to build a method to help with testing and validation, called `debugDump`. This method can iterate the hierarchy of objects stored in the `JSONModel`, and write them to an output stream. Let's imagine we these classes:

```
class Bar {
  float amount;
};
class Foo {
   int value;
   Bar bar;
};
```

That class would be stored in this JSON:

```
{
  "meta" :  {
     "class" : "Foo", 
     "version" : 1.0
   }, 
   "members" : {
      "value" : 1,
      "bar" : {
          "meta" :  {
             "class" : "Bar", 
             "version" : 1.0
           }, 
           "members" : {
              "amount" : 5.95,
           }
      }
   }
}, 

```

Your `JSONReader` will read the JSON (above), and build a corresponding `JSONModel` (internal representation). If we call the `debugDump()` method on your `JSONModel`, the output is a hierarchical (tree) format  that looks like this:

```
(object):
  meta=   (object):
      class=Foo
      version=1.0
  members=   (object):
      value=1
      bar =  (object):
          meta=       (object):
              class=Bar
              version=1.0
          members=  (object):
              amount=5.95 
```

> NOTE: The `Testing.hpp` class has a method called, `doReadTest()` which tests your `JSONReader` for this purpose.  The auto-grader uses this test to grade your `JSONReader` class in a basic read test.

### Challenge #2  -  Implement your `toJSON(std::ostream &aStream)` methods

When we call the `toJSON(std::ostream &aStream) method on any of our `Test` classes, they will write their data members (and some meta informtion) to the given stream in JSON format. Each object writes two sections as JSON,  "meta", and "members".

In the "meta" section -- encode two properties: `class`, and `version`.  The `class` property will contain the name of this class. The `version` property outputs version `1.0`.  

In the "members" section -- encode all the protected data members of this class.   

#### STEP 2A -- Implement `TestClass1::toJSON()`
Implement the `toJSON()` method so it encodes the `meta` and `members` sections for the this class into the given stream.

#### STEP 2B -- Implement `TestClass2::toJSON()`
Implement the `toJSON()` method so it encodes the `meta` and `members` sections for the this class into the given stream.

#### STEP 2C -- Implement `TestClass3::toJSON()`
Implement the `toJSON()` method so it encodes the `meta` and `members` sections for the this class into the given stream.

#### STEP 2D -- Implement `TestDocument::toJSON()`
Implement the `toJSON()` method so it encodes the `meta` and `members` sections for the this class into the given stream.

The `toJSON()` method for this class is slightly more complicated, because the `TestDocument` class has intrinsic data members, as well as object data members.  Refer to the examples in this readme to get the JSON output right.

> NOTE: Once you have your  working, manually **VALIDATE** the JSON you generate is correct by copying the JSON output as text, and pasting it to a JSON validator.  A free/good/simple one can be found at https://jsonformatter.curiousconcept.com/. 


### Challenge #3  - Implment the `Builder` class to reconstruct C++ objects from `JSONModel`

Once your Test classes can write themselves as JSON, you're ready to implement your `Builder` class.  In our testing framework, you'll first encode a `TestDocument` object and its children as JSON into a text stream. Next, our test tries to read the encoded JSON data, and re-construct the `TestDocument` class hierarchy of C++ objects.  Your `Builder` class uses the `JSONModel` to re-construct actual C++ objects. 

#### Step 3A -- Implement the `Builder::create(JSONModel &aModel)` method

There are several comman ways to re-construct C++ objects using a Builder Pattern. For example, you might find it easier to construct the individual `TestClasses` first, then assemble them together in the `TestDocument`.  Try working this out on paper before actually writing code to save yourself time and effort. 

You may freely change the constructors for the `TestClasses` and the `TestDocument` if that helps. You may add additional constructors or other methods as well. You may not change or remove data members from any of these classes -- but you may add your own data members. 

Of course, your `Builder` **MUST** create re-constructed objects using data in the `JSONModel` it is provided.

## Auto-Testing
For our testbed, we've provided a class called `TestDocument`, that contains a number of other classes (`TestClass1..3`). We provide three tests for you -- but you should create your own tests too -- to make sure your code works correctly.

### Compile Test - 10pts
Just verifies that you code can compile. 

### Reading Test  (JSON decode) - 15pts
In our reading test, we will open a basic JSON file (stream), and try to parse the contents with your `JSONReader`. As you read data, you will build up and internal model of type `JSONModel` -- which is a collection of `JSONList`, `JSONObject` and `JSONElement`.  We verify that your `JSONReader` is generally working by scanning your output and verifing certain fields are present.

### Write Test (JSON encode) - 15pts

In the writing test, we create a `TestDocument` object that contains a variety of simple fields.  Your task is to persist (save) the `TestDocument` and its nested classes as JSON.  A successful outcome occurs when the output stream contains a valid JSON representation of the `TestDocument` and its child objects.  Our validation for this test is minimal, just ensuring that we see specific fields in the output stream. 

> As a reminder, when you encode your object data properties into JSON, **MAKE SURE** that the fields are written out in the same order they are declared in the class.

### Round-trip Test - 55pts
This test validates whether you can correctly read/write objects in JSON format. This test fully validates your `Builder`, `JSONReader` and `JSONModel` are working correctly. The test proceeds as follows:

1. We begin by creating a new `TestDocument` object (with some child objects) for you
2. We write that object (and children) to a stream using the `toJSON` methods (see writing test above)
3. Next we read from the stream from step #2 using your `JSONReader`, to build a `JSONModel` model
4. Then we call your `Builder` to create a clone of our original `TestDocument` using your `JSONModel` from step 3
5. Finally, we verify that the original `TestDocument` and clone are equal. If so...take a break, you're done!

-- Another 5points awarded based on manual code check for style and comments.

## Submitting Your Work
### Due Feb 20, 2021 -- at 11:30pm (PST)

Vlad-the-compiler is eagerly awaiting your code submission. Don't forget to update your `student.json` file with the relevant information before turning in your work.

Good luck!
