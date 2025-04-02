#include <functional>
#include <memory>

namespace gait_detection
{
    namespace fuzzy_logic
    {
        struct FuzzySet
        {
            int crisp_value;
            std::unique_ptr<std::function<double(int)>> membership_func;
            double calc_membership_value();
        };

        }
}