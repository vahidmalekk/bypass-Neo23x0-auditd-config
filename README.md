# Bypass Neo23x0 auditd Config with execveat Syscall for process creation
This repository provides a Proof of Concept (PoC) demonstrating how the execveat syscall can be used to bypass the Neo23x0 auditd configuration for process creation. The PoC highlights potential gaps in the default auditd configuration and how attackers can evade detection by using alternative syscalls that aren't typically monitored.
## Purpose
The Neo23x0 auditd configuration is designed to capture key security events on Linux systems, particularly focusing on process execution via the execve syscall. However, with the introduction of the execveat syscall, which is an extension of execve that allows executing a file relative to a directory file descriptor, it’s possible to bypass the monitoring of process creation events.

This PoC demonstrates how to exploit this syscall to execute commands without triggering auditd logging, emphasizing the need for more comprehensive audit rules.
## Requirements
A Linux system running with auditd installed and Neo23x0 auditd configuration applied.
Root privileges to modify auditd configuration and execute elevated system calls.
## Setup Instructions
### 1. Review and Apply Neo23x0 auditd Configuration:
This PoC assumes you're using the Neo23x0 auditd configuration. If you haven't applied it, follow the instructions from the official Neo23x0 repository:

Review or apply the auditd rules located at /etc/audit/rules.d/ according to Neo23x0's guidelines.
````bash
git clone https://github.com/Neo23x0/auditd.git
cp auditd/audit.rules /etc/audit/rules.d/
````
### 2.  Build the PoC Binary and test the bypass:

````bash
git clone https://github.com/vahidmalekk/bypass-Neo23x0-auditd-config.git
cd bypass-Neo23x0-auditd-config
gcc poc.c -o poc
./poc
````
### 3. Check Audit Logs:
After executing the binary, review the audit logs in /var/log/audit/audit.log to determine whether the process creation of (unmae,id,whoami,cat /etc/passwd) was logged or you just see poc executed?

