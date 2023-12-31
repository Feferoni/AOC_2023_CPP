#include <algorithm>
#include <numeric>
#include <ranges>
#include <regex>

#include "day4Impl.h"

#include "common/inc/errorMsg.h"
#include "common/inc/inputHelper.h"
#include "common/inc/stringHelper.h"

namespace {

constexpr char cardPattern[] = "Card\\s+(\\d+): (.*)";

struct CardData {
    uint32_t cardNumber          = 0;
    uint32_t totalWinningNumbers = 0;

    [[maybe_unused]] friend std::ostream& operator<<(std::ostream& os, const CardData& cd) {
        os << "CardNumber: " << cd.cardNumber << " totalWinningNumbers: " << cd.totalWinningNumbers;
        return os;
    };
};

[[nodiscard]] auto parseStringOfNumbers(const std::vector<std::string>& stringsOfNumbers) -> std::vector<uint32_t> {
    std::vector<uint32_t> numbers;

    auto isStrEmpty = [](const std::string& str) { return !str.empty(); };
    auto pushBack   = [&numbers](const std::string& str) {
        numbers.push_back(std::stoul(str));
    };

    std::ranges::for_each(stringsOfNumbers | std::views::filter(isStrEmpty), pushBack);

    return numbers;
}

[[nodiscard]] auto getWinningNumbersCount(const auto& winningNumbers, const auto& yourNumbers) -> uint32_t {
    uint32_t score = 0;
    std::ranges::for_each(winningNumbers, [&](const uint32_t winningNumber) {
        const auto haveWinningNumber = std::ranges::find(yourNumbers, winningNumber) !=
                                       yourNumbers.end();
        if (haveWinningNumber) {
            score += 1;
        }
    });

    return score;
}

[[nodiscard]] auto parseStringToCard(const std::string& inputLine) -> CardData {
    CardData cardData;

    std::smatch cardMatch;
    if (!std::regex_search(inputLine, cardMatch, std::regex(cardPattern)) ||
        cardMatch.empty()) {
        ERROR_MSG_AND_EXIT("Could not extract pattern: " << cardPattern << " from: " << inputLine);
    }

    cardData.cardNumber = std::stoul(cardMatch[1]);

    const auto& allNumbersStr = cardMatch[2];
    const auto  splitNumbers  = helper::string::splitString(allNumbersStr, "|");
    if (splitNumbers.size() != 2) {
        ERROR_MSG_AND_EXIT("Split numbers is wrong: " << splitNumbers);
    }

    const auto winningNumberStrings = helper::string::splitString(splitNumbers[0], " ");
    const auto yourNumbersStrings = helper::string::splitString(splitNumbers[1], " ");
    const auto winningNumbers = parseStringOfNumbers(winningNumberStrings);
    const auto yourNumbers    = parseStringOfNumbers(yourNumbersStrings);
    cardData.totalWinningNumbers = getWinningNumbersCount(winningNumbers, yourNumbers);

    return cardData;
}

[[nodiscard]] auto parseInputToCards(const std::vector<std::string>& input) -> std::vector<CardData> {
    std::vector<CardData> parsedCards;
    std::ranges::transform(input, std::back_inserter(parsedCards), parseStringToCard);
    return parsedCards;
}

[[nodiscard]] auto getCardScore(const CardData& cardData) -> uint32_t {
    std::uint32_t score = 0;
    for (auto i : std::views::iota(0u, cardData.totalWinningNumbers)) {
        score = (i == 0) ? 1 : score * 2;
    }
    return score;
}
}  // namespace

auto Day4::getInstance() -> std::unique_ptr<Day4> {
    return std::make_unique<Day4Impl>();
}

auto Day4Impl::part1() -> std::string {
    const auto input = helper::input::getInput<Day4>(std::source_location::current());
    const auto cards       = parseInputToCards(input);
    uint32_t   totaltScore = 0;
    std::ranges::for_each(cards, [&totaltScore](const CardData& cardData) {
        totaltScore += getCardScore(cardData);
    });
    return std::to_string(totaltScore);
};

auto Day4Impl::part2() -> std::string {
    const auto input = helper::input::getInput<Day4>(std::source_location::current());
    const auto            cards = parseInputToCards(input);
    std::vector<uint32_t> cardCount(cards.size(), 1);

    for (uint32_t i = 0; i < cardCount.size(); i++) {
        const auto& currentCard = cards.at(i);
        for (uint32_t j = i + 1;
             j <= (i + currentCard.totalWinningNumbers) && j < cardCount.size(); j++) {
            cardCount[j] += cardCount[i];
        }
    }

    uint32_t totalCards = std::accumulate(cardCount.begin(), cardCount.end(), 0);
    return std::to_string(totalCards);
};
