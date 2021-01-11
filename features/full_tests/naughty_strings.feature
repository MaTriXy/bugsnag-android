Feature: The notifier handles user data containing unusual strings

#Scenario: Test handled JVM error
#    When I run "NaughtyStringScenario"
#    Then I wait to receive a request
#    And the request is valid for the error reporting API version "4.0" for the "Android Bugsnag Notifier" notifier
#    And the payload field "events" is an array with 1 elements
#    And the exception "errorClass" equals "java.lang.RuntimeException"
#    And the exception "message" equals "NaughtyStringScenario"
#    And the payload field "events.0.metaData.custom" is not null
#    And the payload field "events.0.metaData.custom.val_1" equals "ЁЂЃЄЅІЇЈЉЊЋЌЍЎЏАБВГДЕЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯабвгдежзийклмнопрстуфхцчшщъыьэюя"
#    And the payload field "events.0.metaData.custom.val_2" equals "田中さんにあげて下さい"
#    And the payload field "events.0.metaData.custom.val_3" equals "𐐜 𐐔𐐇𐐝𐐀𐐡𐐇𐐓 𐐙𐐊𐐡𐐝𐐓/𐐝𐐇𐐗𐐊𐐤𐐔 𐐒𐐋𐐗 𐐒𐐌 𐐜 𐐡𐐀𐐖𐐇𐐤𐐓𐐝 𐐱𐑂 𐑄 𐐔𐐇𐐝𐐀𐐡𐐇𐐓 𐐏𐐆𐐅𐐤𐐆𐐚𐐊𐐡𐐝𐐆𐐓𐐆"
#    And the payload field "events.0.metaData.custom.val_4" equals "表ポあA鷗ŒéＢ逍Üßªąñ丂㐀𠀀"
#    And the payload field "events.0.metaData.custom.val_5" equals ",。・:*:・゜’( ☻ ω ☻ )。・:*:・゜’"
#    And the payload field "events.0.metaData.custom.val_6" equals "❤️ 💔 💌 💕 💞 💓 💗 💖 💘 💝 💟 💜 💛 💚 💙"
#    And the payload field "events.0.metaData.custom.val_7" equals "✋🏿 💪🏿 👐🏿 🙌🏿 👏🏿 🙏🏿"
#    And the payload field "events.0.metaData.custom.val_8" equals "👨‍👩‍👦 👨‍👩‍👧‍👦 👨‍👨‍👦 👩‍👩‍👧 👨‍👦 👨‍👧‍👦 👩‍👦 👩‍👧‍👦"
#    And the payload field "events.0.metaData.custom.val_9" equals "🚾 🆒 🆓 🆕 🆖 🆗 🆙 🏧"
#    And the payload field "events.0.metaData.custom.val_10" equals "１２３"
#    And the payload field "events.0.metaData.custom.val_11" equals "الكل في المجمو عة (5)"
#    And the payload field "events.0.metaData.custom.val_12" equals "˙ɐnbᴉlɐ ɐuƃɐɯ ǝɹolop ʇǝ ǝɹoqɐl ʇn ʇunpᴉpᴉɔuᴉ ɹodɯǝʇ poɯsnᴉǝ"
#    And the payload field "events.0.metaData.custom.val_13" equals "𝓣𝓱𝓮 𝓺𝓾𝓲𝓬𝓴 𝓫𝓻𝓸𝔀𝓷 𝓯𝓸𝔁 𝓳𝓾𝓶𝓹𝓼 𝓸𝓿𝓮𝓻 𝓽𝓱𝓮 𝓵𝓪𝔃𝔂 𝓭𝓸𝓰"
#    And the payload field "events.0.metaData.custom.val_14" equals "گچپژ"

# commented out some failing unicode assertions and skipped Android 4.4 until PLAT-5606 is addressed
@skip_below_android_5
Scenario: Test unhandled NDK error
    When I run "CXXNaughtyStringsScenario" and relaunch the app
    And I configure the app to run in the "non-crashy" state
    And I configure Bugsnag for "CXXNaughtyStringsScenario"
    Then I wait to receive a request
    And the request is valid for the error reporting API version "4.0" for the "Android Bugsnag Notifier" notifier
    And the payload field "events" is an array with 1 elements
    And the exception "errorClass" equals "SIGABRT"
    And the exception "message" equals "Abort program"
    And the payload field "events.0.metaData.custom" is not null
#    And the payload field "events.0.metaData.custom.val_1" equals "ЁЂЃЄЅІЇЈЉЊЋЌЍЎЏАБВГДЕЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯабвгдежзийклмнопрстуфхцчшщъыьэюя"
    And the payload field "events.0.metaData.custom.val_2" equals "田中さんにあげて下さい"
#    And the payload field "events.0.metaData.custom.val_3" equals "𐐜 𐐔𐐇𐐝𐐀𐐡𐐇𐐓 𐐙𐐊𐐡𐐝𐐓/𐐝𐐇𐐗𐐊𐐤𐐔 𐐒𐐋𐐗 𐐒𐐌 𐐜 𐐡𐐀𐐖𐐇𐐤𐐓𐐝 𐐱𐑂 𐑄 𐐔𐐇𐐝𐐀𐐡𐐇𐐓 𐐏𐐆𐐅𐐤𐐆𐐚𐐊𐐡𐐝𐐆𐐓𐐆"
    And the payload field "events.0.metaData.custom.val_4" equals "表ポあA鷗ŒéＢ逍Üßªąñ丂㐀𠀀"
    And the payload field "events.0.metaData.custom.val_5" equals ",。・:*:・゜’( ☻ ω ☻ )。・:*:・゜’"
#    And the payload field "events.0.metaData.custom.val_6" equals "❤️ 💔 💌 💕 💞 💓 💗 💖 💘 💝 💟 💜 💛 💚 💙"
    And the payload field "events.0.metaData.custom.val_7" equals "✋🏿 💪🏿 👐🏿 🙌🏿 👏🏿 🙏🏿"
#    And the payload field "events.0.metaData.custom.val_8" equals "👨‍👩‍👦 👨‍👩‍👧‍👦 👨‍👨‍👦 👩‍👩‍👧 👨‍👦 👨‍👧‍👦 👩‍👦 👩‍👧‍👦"
    And the payload field "events.0.metaData.custom.val_9" equals "🚾 🆒 🆓 🆕 🆖 🆗 🆙 🏧"
    And the payload field "events.0.metaData.custom.val_10" equals "１２３"
    And the payload field "events.0.metaData.custom.val_11" equals "الكل في المجمو عة (5)"
#    And the payload field "events.0.metaData.custom.val_12" equals "˙ɐnbᴉlɐ ɐuƃɐɯ ǝɹolop ʇǝ ǝɹoqɐl ʇn ʇunpᴉpᴉɔuᴉ ɹodɯǝʇ poɯsnᴉǝ"
#    And the payload field "events.0.metaData.custom.val_13" equals "𝓣𝓱𝓮 𝓺𝓾𝓲𝓬𝓴 𝓫𝓻𝓸𝔀𝓷 𝓯𝓸𝔁 𝓳𝓾𝓶𝓹𝓼 𝓸𝓿𝓮𝓻 𝓽𝓱𝓮 𝓵𝓪𝔃𝔂 𝓭𝓸𝓰"
    And the payload field "events.0.metaData.custom.val_14" equals "گچپژ"
