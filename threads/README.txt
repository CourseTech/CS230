All requirements are satisfied. I have basic messages that indicate the status of every thread at different stages of their execution. 
Thread creation and joining is handled in the main function, and individual phone call logic is handled in each thread's phonecall funciton.
Implementation boils down to two functions–main and phonecall. There is a binary semaphone that 
protects the connection variables's updates, and there is an operator counting semaphore that handles medical orders.
