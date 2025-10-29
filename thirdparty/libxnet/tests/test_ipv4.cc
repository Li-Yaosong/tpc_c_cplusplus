#include <iostream>
#include <cassert>
#include <vector>
#include <string>
#include <array>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wall"
#pragma GCC diagnostic ignored "-Wextra"
#pragma GCC diagnostic ignored "-Wpedantic"
#pragma GCC diagnostic ignored "-Werror"

#include "../include/xnet/IPv4.hh"

#pragma GCC diagnostic pop

void test_ipv4_address_basic()
{
    std::cout << "=== Testing IPv4 Address Basic Functionality ===" << std::endl;
    xnet::IPv4::Address addr1(192, 168, 1, 1);
    xnet::IPv4::Address addr2(10, 0, 0, 1);
    xnet::IPv4::Address addr3(192, 168, 1, 1);

    auto data1 = addr1.data_msbf();
    assert(std::to_integer<uint8_t>(data1[0]) == 192);
    assert(std::to_integer<uint8_t>(data1[1]) == 168);
    assert(std::to_integer<uint8_t>(data1[2]) == 1);
    assert(std::to_integer<uint8_t>(data1[3]) == 1);

    assert(addr1 == addr3);
    assert(!(addr1 == addr2));
    std::array<uint8_t, 4> octets = {172, 16, 0, 1};
    xnet::IPv4::Address addr4(octets);
    auto data4 = addr4.data_msbf();
    assert(std::to_integer<uint8_t>(data4[0]) == 172);
    std::cout << "All address basic tests passed" << std::endl;
}

void test_ipv4_flags()
{
    std::cout << "=== Testing IPv4 Flags ===" << std::endl;
    xnet::IPv4::Flags flags_default;
    assert(!flags_default.dont_fragment());
    assert(!flags_default.more_fragments());
    assert(!flags_default.reserved());
    assert(flags_default.may_fragment());
    assert(flags_default.last_fragment());

    xnet::IPv4::Flags flags_df(0b010);
    assert(flags_df.dont_fragment());
    assert(!flags_df.may_fragment());
    assert(flags_df.last_fragment());

    xnet::IPv4::Flags flags_mf(0b001);
    assert(flags_mf.more_fragments());
    assert(!flags_mf.last_fragment());

    xnet::IPv4::Flags flags_rsv(0b100);
    assert(flags_rsv.reserved());
    std::cout << "All flags tests passed" << std::endl;
}

void test_ipv4_header_serialization()
{
    std::cout << "=== Testing IPv4 Header Serialization ===" << std::endl;
    xnet::IPv4::Header header;
    header.header_size = 20;
    header.TOS_or_DS = 0;
    header.total_size = 60;
    header.identification = 12345;
    header.flags = xnet::IPv4::Flags(0b010);
    header.fragment_offset = 0;
    header.time_to_live = 64;
    header.protocol = 6;
    header.checksum = 0;
    header.source_address = xnet::IPv4::Address(192, 168, 1, 100);
    header.destination_address = xnet::IPv4::Address(8, 8, 8, 8);

    auto serialized = xnet::IPv4::serialize(header);
    assert(serialized.size() == xnet::IPv4::minimal_header_size);

    xnet::IPv4::HeaderView header_view(serialized);
    auto parsed_header = header_view.parse();

    assert(parsed_header.has_value());
    auto &parsed = parsed_header.value();

    assert(parsed.header_size == header.header_size);
    assert(parsed.TOS_or_DS == header.TOS_or_DS);
    assert(parsed.total_size == header.total_size);
    assert(parsed.identification == header.identification);
    assert(parsed.flags.value() == header.flags.value());
    assert(parsed.fragment_offset == header.fragment_offset);
    assert(parsed.time_to_live == header.time_to_live);
    assert(parsed.protocol == header.protocol);
    assert(parsed.source_address == header.source_address);
    assert(parsed.destination_address == header.destination_address);

    std::cout << "Header serialization/deserialization tests passed" << std::endl;
}

void test_ipv4_checksum()
{
    std::cout << "=== Testing IPv4 Checksum ===" << std::endl;
    xnet::IPv4::Header header;
    header.header_size = 20;
    header.TOS_or_DS = 0;
    header.total_size = 40;
    header.identification = 54321;
    header.flags = xnet::IPv4::Flags(0);
    header.fragment_offset = 0;
    header.time_to_live = 128;
    header.protocol = 17;
    header.checksum = 0;
    header.source_address = xnet::IPv4::Address(10, 0, 0, 2);
    header.destination_address = xnet::IPv4::Address(10, 0, 0, 1);

    auto checksum = xnet::IPv4::compute_checksum(header);
    header.checksum = checksum;

    auto serialized = xnet::IPv4::serialize(header);
    xnet::IPv4::HeaderView header_view(serialized);

    assert(header_view.verify_checksum());

    header.checksum = 0xFFFF;
    auto bad_serialized = xnet::IPv4::serialize(header);
    xnet::IPv4::HeaderView bad_header_view(bad_serialized);
    assert(!bad_header_view.verify_checksum());

    std::cout << "Checksum tests passed" << std::endl;
}

void test_special_addresses()
{
    std::cout << "=== Testing Special Addresses ===" << std::endl;

    xnet::IPv4::Address loopback(127, 0, 0, 1);
    auto loopback_data = loopback.data_msbf();
    assert(std::to_integer<uint8_t>(loopback_data[0]) == 127);

    xnet::IPv4::Address broadcast(255, 255, 255, 255);
    auto broadcast_data = broadcast.data_msbf();
    assert(std::to_integer<uint8_t>(broadcast_data[3]) == 255);

    xnet::IPv4::Address zero(0, 0, 0, 0);
    auto zero_data = zero.data_msbf();
    assert(std::to_integer<uint8_t>(zero_data[0]) == 0);

    xnet::IPv4::Address max_vals(255, 255, 255, 254);
    auto max_data = max_vals.data_msbf();
    assert(std::to_integer<uint8_t>(max_data[0]) == 255);

    std::cout << "Special addresses tests passed" << std::endl;
}

void test_protocol_field()
{
    std::cout << "=== Testing Protocol Field ===" << std::endl;

    xnet::IPv4::Header header;
    header.header_size = 20;
    header.TOS_or_DS = 0;
    header.total_size = 20;
    header.identification = 9999;
    header.flags = xnet::IPv4::Flags(0);
    header.fragment_offset = 0;
    header.time_to_live = 64;
    header.checksum = 0;
    header.source_address = xnet::IPv4::Address(1, 1, 1, 1);
    header.destination_address = xnet::IPv4::Address(2, 2, 2, 2);

    header.protocol = 1;
    header.checksum = xnet::IPv4::compute_checksum(header);
    auto icmp_data = xnet::IPv4::serialize(header);
    xnet::IPv4::HeaderView icmp_view(icmp_data);
    assert(icmp_view.protocol().value() == 1);

    header.protocol = 6;
    header.checksum = xnet::IPv4::compute_checksum(header);
    auto tcp_data = xnet::IPv4::serialize(header);
    xnet::IPv4::HeaderView tcp_view(tcp_data);
    assert(tcp_view.protocol().value() == 6);

    header.protocol = 17;
    header.checksum = xnet::IPv4::compute_checksum(header);
    auto udp_data = xnet::IPv4::serialize(header);
    xnet::IPv4::HeaderView udp_view(udp_data);
    assert(udp_view.protocol().value() == 17);

    std::cout << "Protocol field tests passed" << std::endl;
}

int main()
{
    std::cout << "Starting IPv4 Library Tests..." << std::endl;
    std::cout << "=================================" << std::endl;

    try
    {
        test_ipv4_address_basic();
        test_ipv4_flags();
        test_ipv4_header_serialization();
        test_ipv4_checksum();
        test_special_addresses();
        test_protocol_field();

        std::cout << "\n ALL TESTS PASSED! " << std::endl;
        std::cout << "IPv4 library is functioning correctly." << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "\n TEST FAILED: " << e.what() << std::endl;
        return 1;
    }
    catch (...)
    {
        std::cerr << "\n TEST FAILED: Unknown exception" << std::endl;
        return 1;
    }

    return 0;
}