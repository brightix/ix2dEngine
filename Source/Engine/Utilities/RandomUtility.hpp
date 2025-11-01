#pragma once
#include <random>
#include <unordered_map>

#include "FuncLib/ixStaticFuncLib.hpp"
#include "FuncLib/SystemLib.hpp"


static double Rand()
{
	// 静态保证每次调用不会重新初始化随机数生成器
	static std::mt19937 rng(std::random_device{}());
	static std::uniform_real_distribution<double> dist(0.0, 1.0);
	return dist(rng);
}

class RandomUtility final : public EngineSubSystem
{
    uint32_t seed;
    std::mt19937 main_seed;
    std::unordered_map<size_t, std::pair<std::mt19937,std::uniform_int_distribution<int>>> map;
    std::uniform_int_distribution<int> reg_seed{0, INT32_MAX};
public:
    explicit RandomUtility() : seed(std::random_device{}())
    {
	    CNAME;
    }

    std::optional<int> GetRandom(const std::string& reg_name)
    {
        const size_t key = ix::Hash(reg_name.c_str());
        auto it = map.find(key);
        if (it == map.end())
        {
            Log("未能找到 \"" + reg_name + "\"，请先注册");
            return std::nullopt;
        }
        auto& [engine, dist] = it->second;
    	int ret = dist(engine);
        return ret;
    }

    bool RegisterRandom(const std::string &reg_name, std::pair<int,int> min_max)
    {
        size_t hash = ix::Hash(reg_name.c_str());
        auto it = map.find(hash);
        if (it != map.end())
        {
            Log("随机数重名，拒绝生成");
            return false;
        }
        auto& [min,max] = min_max;
        map.emplace(hash,std::make_pair(std::mt19937(reg_seed(main_seed)),std::uniform_int_distribution<int>(min,max)));
        return true;
    }
    void Init() override
    {
        main_seed = std::mt19937(seed);
    }
    //注意调用
    void SetSeed(size_t user_seed)
    {
        seed = user_seed;
    }
    ~RandomUtility(){}
};
