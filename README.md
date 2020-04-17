# OJudge Platform

## What is OJudge?
OJudge Platform is a software for the creation of an *Online Judge* site, such as the famous https://onlinejudge.org. At this moment it is under heavy development, which means that it is mostly unuseable, and the source code is provided here mostly for developers willing to contribute to it.

## License
The code is released under the terms of the GPL-3 license, a copy of which is available in the file LICENSE. According to the license, it comes with ABSOLUTELY NO WARRANTY, and you are free to reuse and distrubute it, in compliance with the terms of the mentioned license.

## Install
Instructions for the building and use of the software:

### Dependencies
To build and run this software you need Wt (http://webtoolkit.eu) and its dependencies (including boost, cmake and a C++17 compatible compiler). Check the Wt documentation on how to install it in your system.

You will also need PostgreSQL installed and running in your system.

oath-toolkit (https://www.nongnu.org/oath-toolkit/index.html) is required for Two Factor Authentication. It is available for install in all major Linux distributions.

### Building
Clone this repository, or a fork of it, and from its root directory run:

```
$ cmake .
$ make
```

If your system is correctly setup, the code will be built. Then, check that the file `run.sh` points to the correct `Wt/resources` directory (which, by default, is `/usr/share/Wt/resources`, but, depending on your particular setup, could be `/usr/local/share/Wt/resources` or similar).

Now create an empty database, with proper permissions, for OJudge to use.

### Running
Before the first run, create or edit the file `<your_home_directory>/.config/ojudge/config` and include the following lines:

```
[database]
host=<the host of your database server, probably will be localhost>
dbname=<name of the database to use>
user=<username to access that database>
password=<password for the username>
```

Once that is setup, you can start the server by running:

```
$ ./run.sh
```

If there are no errors, you'll access the platform via your web browser, pointing it to ```http://localhost:9090```. An administration user is created by default, with username ```admin``` and also password ```admin```.

## Questions or contributing
In case you have problems setting and running this software, or if you'd like to contribute code to its development, you are welcome to join us in https://groups.google.com/d/forum/ojudge-developers.

You can also contribute with financial support using these channels:

* [Patreon](https://www.patreon.com/onlinejudge "Patreon")
* [PayPal](https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=D7XA3E822BTP8&source=url "PayPal")
* Bitcoin: 1ojudgeapLUjJcnUmze67a4w3TJ6WnPxo
