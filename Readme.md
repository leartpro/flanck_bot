# FLANCK BOT

## SETUP
1. create directory `/libs`
2. checkout `/libs` and run the following commands:

   `git clone https://github.com/leartpro/flanck.git -b libary`

   `git clone https://github.com/brainboxdotcc/DPP.git`
3. reload your cmake
4. checkout `/src` and create a file called `DiscordToken.h`
5. paste the following code into `DiscordToken.h` and insert your Bot-Token
   ```c++
      #ifndef FLANCK_BOT_DISCORDTOKEN_H
      #define FLANCK_BOT_DISCORDTOKEN_H
      #include <string>
      using namespace std;
      struct DiscordToken {
      static string getToken() {
      return "<your token here>";
      }
      };
      #endif //FLANCK_BOT_DISCORDTOKEN_H
   ```
   
## ABOUT

The discord bot uses the flanck library, which is based on the simple branch in the flanck repository.
For more information checkout [Flanck](https://github.com/leartpro/flanck).