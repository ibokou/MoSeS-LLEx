#ifndef FUZZY_H
#define FUZZY_H

#include <algorithm>
#include <array>
#include <cmath>
#include <exception>
#include <stdexcept>

namespace fuzzy::algebra {
  class FuzzyException : public std::exception {
  public:
    FuzzyException(const std::string &message = "") : message(message){};
    const char *what() { return (this->message).c_str(); };

  private:
    std::string message;
  };

  class RangeException : public FuzzyException {
    using FuzzyException::FuzzyException;
  };
  class MatchingTupleException : public FuzzyException {
    using FuzzyException::FuzzyException;
  };
  class IncompatibleSetsException : public FuzzyException {
    using FuzzyException::FuzzyException;
  };

  template <typename... Values>
  class CrispTuple {
  public:
    constexpr CrispTuple(Values... crispValues) : elems{{crispValues...}} {};
    constexpr CrispTuple() = default;
    constexpr auto &operator[](std::size_t n) { return elems[n]; }
    constexpr auto &operator[](std::size_t n) const { return elems[n]; }

  private:
    constexpr static std::size_t size = sizeof...(Values);
    std::array<float, size> elems{};
  };

  template <typename... T>
  class FuzzyElement {
  public:
    constexpr FuzzyElement() : membershipValue(0.0f){};
    constexpr FuzzyElement(CrispTuple<T...> tuple, float membershipValue)
        : tuple(tuple), membershipValue(membershipValue){};
    CrispTuple<T...> tuple;
    mutable float membershipValue;
  };

  template <typename... T>
  auto min(FuzzyElement<T...> a, FuzzyElement<T...> b) {
    return a.membershipValue < b.membershipValue ? a : b;
  }

  template <typename... T>
  auto min_cross(FuzzyElement<T...> a, FuzzyElement<T...> b) {
    return min<T...>(a, b);
  }

  template <typename... T>
  auto max(FuzzyElement<T...> a, FuzzyElement<T...> b) {
    return a.membershipValue > b.membershipValue ? a : b;
  }

  template <std::size_t N, typename... T>
  class FuzzySet {
  public:
    template <typename... Values>
    constexpr FuzzySet(Values... elems) : fuzzyElements{{elems...}} {};
    constexpr auto &operator[](std::size_t n) { return fuzzyElements[n]; }
    constexpr auto &operator[](std::size_t n) const { return fuzzyElements[n]; }

    // Complement
    constexpr auto operator~() const {
      auto complement = *this;
      for (auto &elem : complement.fuzzyElements) {
        elem.membershipValue = 1 - elem.membershipValue;
      }
      return complement;
    }

    // Union
    constexpr auto operator+(const FuzzySet<N, T...> &set) const {
      FuzzySet<N, T...> unionizedSet;
      for (std::size_t i = 0; i < N; i++) {
        unionizedSet.fuzzyElements[i] =
            max(this->fuzzyElements[i], set.fuzzyElements[i]);
      }
      return unionizedSet;
    }

    // Intersection
    constexpr auto operator/(const FuzzySet<N, T...> &set) const {
      FuzzySet<N, T...> intersection;
      for (std::size_t i = 0; i < N; i++) {
        intersection.fuzzyElements[i] =
            min(this->fuzzyElements[i], set.fuzzyElements[i]);
      }
      return intersection;
    }

    // Fuzzy relation (Crossproduct)
    constexpr auto operator*(const FuzzySet<N, T...> &set) const {
      FuzzySet<N, T...> crossProduct;
      for (std::size_t i = 0; i < N; i++) {
        crossProduct.fuzzyElements[i] =
            min_cross(this->fuzzyElements[i], set.fuzzyElements[i]);
      }
      return crossProduct;
    }

  private:
    std::array<FuzzyElement<T...>, N> fuzzyElements{};
  };

  template <std::size_t N>
  class UnaryFuzzySetBuilder {
  private:
    struct MembershipValueLookUpTable {
    public:
      constexpr auto &operator[](const CrispTuple<float> &cmpTuple) {
        return *(std::lower_bound(data_.begin(), data_.end(), cmpTuple,
                                  [](const FuzzyElement<float> &elem,
                                     const CrispTuple<float> &cmpTuple) {
                                    return elem.tuple[0] < cmpTuple[0];
                                  }));
      }
      constexpr auto &operator[](const CrispTuple<float> &cmpTuple) const {
        return *(std::lower_bound(data_.begin(), data_.end(), cmpTuple,
                                  [](const FuzzyElement<float> &elem,
                                     const CrispTuple<float> &cmpTuple) {
                                    return elem.tuple[0] < cmpTuple[0];
                                  }));
      }

      constexpr MembershipValueLookUpTable() = default;
      constexpr MembershipValueLookUpTable(
          const CrispTuple<float> &start, const CrispTuple<float> &end,
          const CrispTuple<float> &step,
          float (*membershipFunction)(const float &, const float &,
                                      const float &)) {

        auto ptr = start[0];
        for (std::size_t i = 0; i < size(); i++) {
          auto crispTuple = CrispTuple(ptr);
          auto membershipValue =
              membershipFunction(crispTuple[0], start[0], end[0]);
          data_[i] = FuzzyElement<float>(crispTuple, membershipValue);
          ptr += step[0];
        }
      }

      constexpr auto size() const { return data_.size(); }

    private:
      std::array<FuzzyElement<float>, (N + 2)> data_{};
    };

  public:
    constexpr UnaryFuzzySetBuilder() = default;
    constexpr UnaryFuzzySetBuilder(const CrispTuple<float> &start,
                                   const CrispTuple<float> &end,
                                   const CrispTuple<float> &step,
                                   float (*membershipFunction)(const float &,
                                                               const float &,
                                                               const float &)) {

      if (static_cast<std::size_t>((end[0] - start[0]) / step[0]) > N) {
        throw RangeException("The parameters start, end and steps require more "
                             "space. Increase template paramter N");
      }

      lookUpTable =
          MembershipValueLookUpTable(start, end, step, membershipFunction);
    };

    constexpr auto create(const CrispTuple<float> &tuple) const {
      return FuzzySet<1, float>(lookUpTable[tuple]);
    };

  private:
    MembershipValueLookUpTable lookUpTable{};
  };
}

namespace fuzzy::tsukamoto::inference {
  enum Inference { MaxMinInference, MaxProdInference, Force_Compile_Error };
  enum Defuzzification { CenterofGravity };

  struct RuleOutputTuple {
    float crispValue;
    float membershipValue;
    float area;

    constexpr RuleOutputTuple()
        : crispValue(0.0f), membershipValue(0.0f), area(0.0f){};
    constexpr RuleOutputTuple(float crisp, float membershipVal, float A)
        : crispValue(crisp), membershipValue(membershipVal), area(A){};

    constexpr RuleOutputTuple &operator=(const RuleOutputTuple &) = default;
  };

  template <class T, std::size_t NSets>
  class Rule {
  private:
    struct RuleOutputLookUpTable {
    public:
      constexpr auto &operator[](const float &membershipValue) {
        return *(std::lower_bound(
#ifdef RULE_REVERSE
            data_.begin(), data_.end(),
#else
            data_.begin(), data_.end(),
#endif
            membershipValue,
            [](const RuleOutputTuple &tuple, const float &membershipValue) {
              return tuple.membershipValue < membershipValue;
            }));
      }
      constexpr auto &operator[](const float &membershipValue) const {
        return *(std::lower_bound(
#ifdef RULE_REVERSE
            data_.begin(), data_.end(),
#else
            data_.begin(), data_.end(),
#endif
            membershipValue,
            [](const RuleOutputTuple &tuple, const float &membershipValue) {
              return tuple.membershipValue < membershipValue;
            }));
      }

      constexpr RuleOutputLookUpTable() = default;
      constexpr RuleOutputLookUpTable(
          Inference infType, const float &start, const float &end,
          float (*outputMembershipFunction)(const float &, const float &,
                                            const float &)) {

        switch (infType) {
          case MaxMinInference:
            {
              auto A = 0.0f;
              auto step = (end - start) / N;
              auto crispValue = start;
              for (std::size_t i = 0; i < N + 2; i++) {
                crispValue += step;
                auto const membershipValue =
                    outputMembershipFunction(crispValue, start, end);
                A += (i == 0)
                         ? membershipValue * (crispValue - start)
                         : (membershipValue - data_[i - 1].membershipValue) *
                               (crispValue - start);

                data_[i] = RuleOutputTuple(crispValue, membershipValue, A);
              }

#ifdef RULE_REVERSE
              auto first = data_.begin();
              auto last = data_.end();

              while (first != last && first != --last) {
                std::swap(first->membershipValue, last->membershipValue);
                std::swap(first->area, last->area);
                std::iter_swap(first++, last);
              }

#endif
              break;
            }
          case MaxProdInference:
            {
            }
          default:
            {
              throw std::logic_error("Inference Type does not exist");
            }
        }
      }

      constexpr auto size() const { return data_.size(); }

    private:
      static const std::size_t N = 1000;
      std::array<RuleOutputTuple, N + 2> data_{};
    };

  public:
    template <class... Ts>
    constexpr Rule(Inference infType, const float &start, const float &end,
                   float (*outputMembershipFunction)(const float &,
                                                     const float &,
                                                     const float &)) {
      lookUpTable =
          RuleOutputLookUpTable(infType, start, end, outputMembershipFunction);
    };

    virtual RuleOutputTuple fire(std::array<T, NSets> sets) const = 0;

    RuleOutputLookUpTable lookUpTable{};
  };

  class Defuzzifier {
  public:
    template <std::size_t N>
    static float defuzzify(Defuzzification method,
                           std::array<RuleOutputTuple, N> outputs) {
      auto numerator = 0.0f;
      auto denominator = 0.0f;
      switch (method) {
        case CenterofGravity:
          {
            for (auto &output : outputs) {
              numerator += output.area * output.crispValue;
              denominator += output.area;
            }
          }
          break;
      }
      return (numerator / denominator);
    }
  };

  template <typename... Set>
  auto AND(Set... sets) {
    return (sets * ...);
  }

  template <typename... Set>
  auto OR(Set... sets) {
    return (sets / ...);
  }

  template <class T, std::size_t Res, std::size_t NSets, std::size_t NRules>
  class UnaryFuzzyInferenceSystem {
  public:
    constexpr UnaryFuzzyInferenceSystem(
        const fuzzy::algebra::CrispTuple<float> &start,
        const fuzzy::algebra::CrispTuple<float> &end,
        const fuzzy::algebra::CrispTuple<float> &step,
        float (*membershipFunction)(const float &, const float &,
                                    const float &)) {
      builder = fuzzy::algebra::UnaryFuzzySetBuilder<Res>(start, end, step,
                                                          membershipFunction);
    };

    const UnaryFuzzyInferenceSystem *
    fuzzify(std::array<fuzzy::algebra::CrispTuple<float>, NSets> tuples) const {
      for (size_t i = 0; i < NSets; i++) {
        varsSet[i] = (T)builder.create(tuples[i]);
      }
      return this;
    }

    const UnaryFuzzyInferenceSystem *
    infer(std::array<const Rule<T, NSets> *, NRules> rules) const {
      for (size_t i = 0; i < NRules; i++) {
        ruleOutputs[i] = rules[i]->fire(varsSet);
      }
      return this;
    }

    auto defuzzify(Defuzzification method) const {
      return Defuzzifier::defuzzify<NRules>(method, ruleOutputs);
    }

  private:
    fuzzy::algebra::UnaryFuzzySetBuilder<Res> builder;
    mutable std::array<T, NSets> varsSet;
    mutable std::array<RuleOutputTuple, NRules> ruleOutputs;
  };
}

#endif