/**
 *
 * @file    base64.hpp
 * @date    28/2/2018
 *
 * IPK project 1
 *
 * Client <-> Server communication
 * Grab data about users and display them.
 *
 * Code copied from:
 *     https://renenyffenegger.ch/notes/development/Base64/Encoding-and-decoding-base-64-with-cpp
 *
 */

#pragma once

#include <string>
#include <vector>

std::string base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len);
std::string base64_decode(std::string const& encoded_string);