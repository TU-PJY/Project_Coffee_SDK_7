# mata_ScriptMaker
mata_ScriptMaker is a tool for creating the structure of script files for read-only use in MGK applications.

After creating the script file structure, you need to write data inside "" (number or string).

Example:

<?xml version="1.0" ?>
<Script>
    <ScriptData1 data="example data" />
    <ScriptData2 data="12345" />
</Script>





# mata_ScriptConverter
mata_ScriptConverter is a tool that encrypts or decrypts script files. 
Encrypted script files can only be used in MGK applications using the same password.

To encrypt or decrypt a script file, you need a password file that follows the structure below.

<?xml version="1.0" ?>
<Script>
    <AES string="" />
    <IV string="" />
</Script>


The "SecurityKeyTemplate.xml" file has this structure. 
Copy the file, save it under a different name, and keep it in a path outside the project.

Before encrypting your files, you must write a password of up to 15 characters in the password file.

Be careful not to lose your password file or have it leaked when distributing the project.