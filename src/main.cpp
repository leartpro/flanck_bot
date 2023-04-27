#include <dpp/dpp.h>
#include "flanck/flanck.h"
#include "DiscordToken.h"

int main() {

    dpp::cluster bot(DiscordToken::getToken());
    bot.on_log(dpp::utility::cout_logger());
    //unregister all commands
    /*
    auto commands = bot.global_commands_get_sync();
    for(auto &command : commands) {
        bot.global_command_delete(command.first);
    }
     */

    /* Executes on ready. */
    bot.on_ready([&bot](const dpp::ready_t &event) {
        if (dpp::run_once<struct register_bot_commands>()) {
            /* Define a slash command. */
            dpp::slashcommand flanck("flanck", "Interprets Flanck Code", bot.me.id);
            flanck.add_option(
                    /* Create a subcommand type option for "dog". */
                    dpp::command_option(dpp::co_sub_command, "interpret_code", "Specifies the Input")
                            .add_option(dpp::command_option(dpp::co_string, "program", "paste your code here", true))
                            .add_option(dpp::command_option(dpp::co_string, "input_flag", "use -b or -a",
                                                            false)).set_auto_complete(true)
                            .add_option(dpp::command_option(dpp::co_string, "output_flag", "use -b or -a",
                                                            false)).set_auto_complete(true)
                            .add_option(
                                    dpp::command_option(dpp::co_string, "user_input", "give your input here", false))
            );

            flanck.add_option(
                    /* Create a subcommand type option for "dog". */
                    dpp::command_option(dpp::co_sub_command, "interpret_file", "Specifies the Input")
                            .add_option(
                                    dpp::command_option(dpp::co_attachment, "program", "attach your .flanck file here",
                                                        true))
                            .add_option(dpp::command_option(dpp::co_string, "input_flag", "use -b or -a",
                                                            false)).set_auto_complete(true)
                            .add_option(dpp::command_option(dpp::co_string, "output_flag", "use -b or -a",
                                                            false)).set_auto_complete(true)
                            .add_option(
                                    dpp::command_option(dpp::co_string, "user_input", "give your input here", false))
            );
            /* Create command */
            bot.global_command_create(flanck);
        }
    });

    /* Use the on_slash command event to look for commands */
    bot.on_slashcommand([](const dpp::slashcommand_t &event) {
        dpp::interaction interaction = event.command;
        dpp::command_interaction cmd_data = interaction.get_command_interaction();
        /* Check if the command is the flanck command. */
        if (interaction.get_command_name() == "flanck") {
            /* Get the sub command */
            auto subcommand = cmd_data.options[0];
            /* Check if the subcommand is "interpret_code" */
            if (subcommand.name == "interpret_code") {
                /* Checks if the subcommand has any options. */
                if (!subcommand.options.empty()) {
                    //init arguments
                    //TODO: if not all arguments are given, the application does not respond
                    // because e.g. options[3] has no value on size 2
                    // use lop instead and evaluate by switch(subcommand.options[1].name)
                    const char* programText = get<string>(subcommand.options[0].value).c_str();
                    bool input_as_binary = get<string>(subcommand.options[1].value) != "-a";
                    bool output_as_binary = get<string>(subcommand.options[2].value) != "-a";
                    string user_input = get<string>(subcommand.options[3].value);
                    vector<Stack> stacks;
                    vector<string> input_stacks;
                    //init user input
                    std::string delimiter = " ";
                    size_t position;
                    std::string token;
                    while ((position = user_input.find(delimiter)) != std::string::npos) {
                        token = user_input.substr(0, position);
                        input_stacks.push_back(token);
                        user_input.erase(0, position + delimiter.length());
                    }
                    input_stacks.push_back(user_input);
                    //init stacks from user input
                    stacks.reserve(input_stacks.size());
                    for (const string& s : input_stacks) {
                        Stack stack = input_as_binary ?
                                      Stack::fromBinaryString(s) :
                                      Stack::fromString(s);
                        stacks.push_back(stack);
                    }
                    //interpret program
                    Lexer lexer(programText);
                    Parser parser(lexer);
                    Interpreter interpreter(parser, stacks);
                    interpreter.interpret();
                    //get result
                    string result = output_as_binary ?
                            interpreter.getOutputStack().toBinaryString() :
                            interpreter.getOutputStack().toString();
                    //return result
                    event.reply(result);
                } else {
                    /* Reply if there were no options */
                    event.reply("suspicious usage");
                }
            }
            /* Check if the subcommand is "cat" */
            if (subcommand.name == "interpret_file") {
                /* Checks if the subcommand is "interpret_file" */
                if (!subcommand.options.empty()) {
                    event.reply("currently unavailable");
                } else {
                    /* Reply if there were no options */
                    event.reply("suspicious usage");
                }
            }
        }
    });

    bot.on_autocomplete([&bot](const dpp::autocomplete_t &event) {
        for (auto &opt: event.options) {
            if (opt.focused) {
                std::string input = std::get<std::string>(opt.value);
                bot.interaction_response_create(event.command.id, event.command.token,
                                                dpp::interaction_response(dpp::ir_autocomplete_reply)
                                                        .add_autocomplete_choice(
                                                                dpp::command_option_choice("-b", "binary flag"))
                                                        .add_autocomplete_choice(
                                                                dpp::command_option_choice("-a", "ascii flag"))
                );
                //bot.log(dpp::ll_debug, "Autocomplete " + opt.name + " with value '" + input + "' in field " + event.name);
                break;
            }
        }
    });

    bot.start(dpp::st_wait);

    return 0;
}