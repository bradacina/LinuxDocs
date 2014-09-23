Installing Oracle java
----------------------

Detect if you have java installed:

java -version
javac - version

Adding APT repositories
-----------------------
sudo apt-get install python-software-properties
sudo add-apt-repository ppa:webupd8team/java
sudo apt-get update

Install Oracle JDK 8
--------------------

sudo apt-get install oracle-java8-installer
sudo apt-get install oracle-java8-set-default

Set JAVA_HOME
-------------

sudo update-alternatives --config java

write down the path where java is installed then

sudo vim /etc/environment

and add at the bottome

JAVA_HOME="<path to java>"

source /etc/environment
