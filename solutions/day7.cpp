#include <algorithm>
#include <map>
#include <optional>
#include <span>

#include "day7.h"

#include "common/inc/errorMsg.h"
#include "common/inc/inputHelper.h"

namespace {
[[maybe_unused]] constexpr std::array<char, 13> cardLabelsPart1 = {
    'A', 'K', 'Q', 'J', 'T', '9', '8', '7', '6', '5', '4', '3', '2'};
[[maybe_unused]] constexpr std::array<char, 13> cardLabelsPart2 = {
    'A', 'K', 'Q', 'T', '9', '8', '7', '6', '5', '4', '3', '2', 'J'};

enum class HandType : uint32_t {
    FIVE_OF_A_KIND  = 0,
    FOUR_OF_A_KIND  = 1,
    FULL_HOUSE      = 2,
    THREE_OF_A_KIND = 3,
    TWO_PAIR        = 4,
    PAIR            = 5,
    HIGH_CARD       = 6
};

[[maybe_unused]] auto operator<<(std::ostream& os, const HandType& ht) -> std::ostream& {
    switch (ht) {
    case HandType::FIVE_OF_A_KIND: os << "FiveOfKind"; break;
    case HandType::FOUR_OF_A_KIND: os << "FourOfKind"; break;
    case HandType::FULL_HOUSE: os << "FullHouse"; break;
    case HandType::THREE_OF_A_KIND: os << "ThreeOfKind"; break;
    case HandType::TWO_PAIR: os << "TwoPair"; break;
    case HandType::PAIR: os << "Pair"; break;
    case HandType::HIGH_CARD: os << "HighCard"; break;
    default:
        ERROR_MSG_AND_EXIT("Faulty handType: " << static_cast<uint32_t>(ht));
    }
    return os;
}

[[nodiscard]] auto getLabelCount(const std::string_view hand) -> std::map<char, uint32_t> {
    std::map<char, uint32_t> labelCount;
    for (const auto& label : hand) {
        labelCount[label]++;
    }
    return labelCount;
}

[[nodiscard]] auto getHandTypePart1(const std::string_view hand) -> HandType {
    const auto labelCount = getLabelCount(hand);
    if (labelCount.size() == 1) return HandType::FIVE_OF_A_KIND;
    if (labelCount.size() == 2) {
        auto isFourOfKind = std::ranges::any_of(labelCount, [](const std::pair<char, uint32_t> labelCount) {
            return labelCount.second == 4;
        });
        return isFourOfKind ? HandType::FOUR_OF_A_KIND : HandType::FULL_HOUSE;
    }
    if (labelCount.size() == 3) {
        auto isThreeOfKind = std::ranges::any_of(labelCount, [](const std::pair<char, uint32_t> labelCount) {
            return labelCount.second == 3;
        });
        return isThreeOfKind ? HandType::THREE_OF_A_KIND : HandType::TWO_PAIR;
    }
    if (labelCount.size() == 4) {
        return HandType::PAIR;
    }
    return HandType::HIGH_CARD;
};

[[nodiscard]] auto getHandTypePart2(const std::string_view hand) -> HandType {
    HandType   handType   = getHandTypePart1(hand);
    const auto labelCount = getLabelCount(hand);

    const std::optional<uint32_t> nrOfJokers = std::invoke([&labelCount]() -> std::optional<uint32_t> {
        const auto it = labelCount.find('J');
        if (it != labelCount.end()) {
            return it->second;
        }
        return std::nullopt;
    });

    if (!nrOfJokers.has_value()) {
        return handType;
    }

    switch (handType) {
    case HandType::FIVE_OF_A_KIND: return HandType::FIVE_OF_A_KIND;
    case HandType::FOUR_OF_A_KIND: {
        if (nrOfJokers.value() == 1) return HandType::FIVE_OF_A_KIND;
        if (nrOfJokers.value() == 4) return HandType::FIVE_OF_A_KIND;
        ERROR_MSG_AND_EXIT("Faulty hand: " << hand);
    }
    case HandType::FULL_HOUSE: {
        if (nrOfJokers.value() == 2) return HandType::FIVE_OF_A_KIND;
        if (nrOfJokers.value() == 3) return HandType::FIVE_OF_A_KIND;
        ERROR_MSG_AND_EXIT("Faulty hand: " << hand);
    }
    case HandType::THREE_OF_A_KIND: {
        if (nrOfJokers.value() == 1) return HandType::FOUR_OF_A_KIND;
        if (nrOfJokers.value() == 3) return HandType::FOUR_OF_A_KIND;
        ERROR_MSG_AND_EXIT("Faulty hand: " << hand);
    }
    case HandType::TWO_PAIR: {
        if (nrOfJokers.value() == 1) return HandType::FULL_HOUSE;
        if (nrOfJokers.value() == 2) return HandType::FOUR_OF_A_KIND;
        ERROR_MSG_AND_EXIT("Faulty hand: " << hand);
    }
    case HandType::PAIR: {
        if (nrOfJokers.value() == 1) return HandType::THREE_OF_A_KIND;
        if (nrOfJokers.value() == 2) return HandType::THREE_OF_A_KIND;
        ERROR_MSG_AND_EXIT("Faulty hand: " << hand);
    }
    case HandType::HIGH_CARD: {
        if (nrOfJokers.value() == 1) return HandType::PAIR;
        ERROR_MSG_AND_EXIT("Faulty hand: " << hand);
    }
    default:
        ERROR_MSG_AND_EXIT("Faulty handType: " << static_cast<uint32_t>(handType));
    }
};

struct CardHand {
    CardHand() = delete;
    explicit CardHand(const std::string& input, const std::function<HandType(const std::string_view)>& createHandTypeFunction, std::span<const char> cardLabels) :
        cardLabels(cardLabels) {
        cards = std::string_view{input.data(), 5};
        try {
            bid = std::stoul(input.substr(6));
        } catch (const std::out_of_range& ex) {
            ERROR_MSG_AND_EXIT(ex.what() << " with input: " << input);
        } catch (const std::invalid_argument& ex) {
            ERROR_MSG_AND_EXIT(ex.what() << " with input: " << input);
        }
        handType = createHandTypeFunction(cards);
    }

    [[nodiscard]] auto operator==(const CardHand& other) const -> bool {
        return cards == other.cards;
    }

    [[nodiscard]] auto operator!=(const CardHand& other) const -> bool {
        return !(*this == other);
    }

    [[nodiscard]] auto operator<(const CardHand& other) const -> bool {
        if (handType == other.handType) {
            return !isStrongerThan(other);
        }

        return handType < other.handType;
    }

    [[maybe_unused]] friend auto operator<<(std::ostream& os, const CardHand& ch) -> std::ostream& {
        os << ch.cards << " - " << ch.handType << " bid: " << ch.bid;
        return os;
    }

    std::span<const char> cardLabels;
    std::string_view      cards;
    HandType              handType;
    uint32_t              bid;

private:
    [[nodiscard]] auto isStrongerThan(const CardHand other) const -> bool {
        const auto getLabelScore = [&](const char label) -> uint32_t {
            for (uint32_t i = 0; i < cardLabels.size(); i++) {
                if (label == cardLabels[i]) return i;
            }
            ERROR_MSG_AND_EXIT("Non valid label: " << label);
        };

        for (uint32_t i = 0; i < 5; i++) {
            const auto thisLabelScore  = getLabelScore(cards.at(i));
            const auto otherLabelScore = getLabelScore(other.cards.at(i));
            if (thisLabelScore == otherLabelScore) {
                continue;
            }
            return thisLabelScore > otherLabelScore;
        }
        ERROR_MSG_AND_EXIT("Could not tiebreak. This: " << *this << " ohter hand: " << other);
    }
};

[[nodiscard]] auto parseInputToCardHands(const std::vector<std::string>& input, const std::function<HandType(const std::string_view)>& createHandTypeFunction, const std::span<const char> cardLabels) -> std::vector<CardHand> {
    std::vector<CardHand> cardHands;
    std::ranges::transform(input, std::back_inserter(cardHands), [&createHandTypeFunction, &cardLabels](const std::string& input) {
        return CardHand(input, createHandTypeFunction, cardLabels);
    });
    return cardHands;
}

[[nodiscard]] auto getSumOfBid(const std::vector<CardHand>& cardHands) -> uint32_t {
    uint32_t sum = 0;
    for (uint32_t i = 0; i < cardHands.size(); i++) {
        sum += (i + 1) * cardHands.at(i).bid;
    }
    return sum;
}
}  // namespace

auto Day7::part1() -> std::string {
    const auto input = helper::input::getInput<Day7>(std::source_location::current());
    auto cardHands = parseInputToCardHands(input, getHandTypePart1, std::span<const char>(cardLabelsPart1));
    std::sort(cardHands.rbegin(), cardHands.rend());
    return std::to_string(getSumOfBid(cardHands));
};

auto Day7::part2() -> std::string {
    const auto input = helper::input::getInput<Day7>(std::source_location::current());
    auto cardHands = parseInputToCardHands(input, getHandTypePart2, std::span<const char>(cardLabelsPart2));
    std::sort(cardHands.rbegin(), cardHands.rend());
    return std::to_string(getSumOfBid(cardHands));
};
