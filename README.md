# oneTimePadEncryptor
A simple application that can generate keys, encrypt, and decipher text files. 

NOTE: only the oneTime.c file is necessary.

The program can take a text file and encrypt it using a pseudo random key, it will output both the key text file and the crypto message,
conversly, the program wkill also take an encrypted file and a key and reverse the encryption. This algorythim is almost 
certainly not compatible with other one-time-pad encrypting tools. That being said, it is a one time pad, all characters are split up 
into 1's and 0's and XOR'ed with the symmetric key. The cryptomessage is thus mathematically impossible to decipher without the key, 
and the key alone does not reveal anything about the message. 
