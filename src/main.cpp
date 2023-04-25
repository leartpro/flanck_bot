#include <dpp/dpp.h>
#include "Token.h"

int main() {

    dpp::cluster bot(Token::getToken());

    bot.on_log(dpp::utility::cout_logger());

    /* Executes on ready. */
    bot.on_ready([&bot](const dpp::ready_t &event) {
        if (dpp::run_once<struct register_bot_commands>()) {
            /* Define a slash command. */
            dpp::slashcommand flanck("flanck", "Interprets Flanck Code", bot.me.id);
            flanck.add_option(
                    /* Create a subcommand type option for "dog". */
                    dpp::command_option(dpp::co_sub_command, "interpret_code", "Specifies the Input")
                            .add_option(dpp::command_option(dpp::co_string, "program", "paste your code here", true))
                            .add_option(dpp::command_option(dpp::co_string, "input_flag", "use -b or -a", false)).set_auto_complete(true)
                            .add_option(dpp::command_option(dpp::co_string, "output_flag", "use -b or -a", false)).set_auto_complete(true)
                            .add_option(dpp::command_option(dpp::co_string, "user_input", "give your input here", false))
            );

            flanck.add_option(
                    /* Create a subcommand type option for "dog". */
                    dpp::command_option(dpp::co_sub_command, "interpret_file", "Specifies the Input")
                            .add_option(dpp::command_option(dpp::co_attachment, "program", "attach your .flanck file here", true))
                            .add_option(dpp::command_option(dpp::co_string, "input_flag", "use -b or -a", false)).set_auto_complete(true)
                            .add_option(dpp::command_option(dpp::co_string, "output_flag", "use -b or -a", false)).set_auto_complete(true)
                            .add_option(dpp::command_option(dpp::co_string, "user_input", "give your input here", false))
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
                    /* Get the user from the parameter */
                    const dpp::user &user = interaction.get_resolved_user(
                            subcommand.get_value<dpp::snowflake>(0)
                    );
                    event.reply(user.get_mention() + " has now been turned into a dog.");
                } else {
                    /* Reply if there were no options */
                    event.reply("No user specified");
                }
            }
            /* Check if the subcommand is "cat" */
            if (subcommand.name == "interpret_file") {
                /* Checks if the subcommand is "interpret_file" */
                if (!subcommand.options.empty()) {
                    /* Get the user from the parameter */
                    const dpp::user &user = interaction.get_resolved_user(
                            subcommand.get_value<dpp::snowflake>(0)
                    );
                    event.reply(user.get_mention() + " has now been turned into a cat.");
                } else {
                    /* Reply if there were no options */
                    event.reply("No user specified");
                }
            }
        }
    });

    bot.on_autocomplete([&bot](const dpp::autocomplete_t & event) {
        for (auto & opt : event.options) {
            /* The option which has focused set to true is the one the user is typing in */
            if (opt.focused) {
                /* In a real world usage of this function you should return values that loosely match
                 * opt.value, which contains what the user has typed so far. The opt.value is a variant
                 * and will contain the type identical to that of the slash command parameter.
                 * Here we can safely know it is string.
                 */
                std::string uservalue = std::get<std::string>(opt.value);
                bot.interaction_response_create(event.command.id, event.command.token, dpp::interaction_response(dpp::ir_autocomplete_reply)
                        .add_autocomplete_choice(dpp::command_option_choice("-b", "binary flag"))
                        .add_autocomplete_choice(dpp::command_option_choice("-a", "ascii flag"))
                );
                bot.log(dpp::ll_debug, "Autocomplete " + opt.name + " with value '" + uservalue + "' in field " + event.name);
                break;
            }
        }
    });

    bot.start(dpp::st_wait);

    return 0;
}