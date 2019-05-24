// consistent_hashing.cpp: 定义应用程序的入口点。
//

#include <iostream>
#include "consistent_hashing.h"

typedef unsigned int UInt32;

struct Node
{
	explicit Node(UInt32 virtual_ip, UInt32 real_ip) : virtual_ip_(virtual_ip),
		real_ip_(real_ip)
	{
	}
	UInt32 virtual_ip_;
	UInt32 real_ip_;
};

struct Hash
{
	Hash() = default;
	/*refer to linux kernel hash.h,just for test*/
	UInt32 operator()(UInt32 ip)
	{
		UInt32 hash = ip;

		/* 2^31 + 2^29 - 2^25 + 2^22 - 2^19 - 2^16 + 1 */
#define GOLDEN_RATIO_PRIME 0x9e370001UL
#define bits 8
		/* On some cpus multiply is faster, on others gcc will do shifts */
		hash *= GOLDEN_RATIO_PRIME;

		/* High bits are more random, so use them. */
		return hash >> (32 - bits);
	}
	UInt32 operator()(const Node & node)
	{
		return operator()(node.virtual_ip_);
	}
};

int main()
{
	ConsistentHashMap<UInt32, Node> h;
	Hash hash_obj;
	UInt32 real_ip_array[] = { 0xC0A83F15, 0xC0A83F65, 0xC0A83F78, 0xC0A83E06 };
	UInt32 real_ip_cnt = sizeof(real_ip_array) / sizeof(real_ip_array[0]);
	UInt32 virtual_ip = 0xC0A80000;

	for (UInt32 i = 0; i < 300; i++)
	{
		h.insert(Node(virtual_ip + i, real_ip_array[i % real_ip_cnt]), hash_obj);
	}
	std::cout << "total nodes count:" << h.size() << " " << sizeof(UInt32) << '\n';

	ConsistentHashMap<UInt32, Node>::const_iterator it = h.find(hash_obj(0xabcdef00));
	if (it != h.end())
		std::cout << "key:" << it->first << " virtual_ip:" << std::hex << "0x" << it->second.virtual_ip_
		<< " real_ip:" << it->second.real_ip_ << std::dec << '\n';

	it = h.find(hash_obj(0x5629870));
	if (it != h.end())
		std::cout << "key:" << it->first << " virtual_ip:" << std::hex << "0x" << it->second.virtual_ip_
		<< " real_ip:" << it->second.real_ip_ << std::dec << std::endl;
	return 0;
}