#!/usr/bin/env ruby

cache_size = 256
block_size = 64

slots = cache_size / block_size

addresses = [
  0xFF0057F2,
  0xFF002311,
  0xEA022000,
  0xEA022011,
  0xFF006682,
  0x00000000,
  0xFF0057EA,
  0xEA022015
]
addresses.map! { |a| a.to_s(2).rjust(8, '0') }

cache = {}

# direct mapping

  puts "4 sets: 2 bit index"
  puts "64 bytes in block: 6 bit offset"
addresses.each do |addr|
  b_tag = addr[..-8].to_i(2).to_s(16).rjust(7, '0')
  b_index = addr [-8..-7].rjust(2, '0')
  b_offset = addr[-6..].rjust(6, '0')

  replace = !!cache[b_index]
  cache[b_index] = addr

  # address tag index offset replace
  puts "`#{addr.to_i(2).to_s(16).rjust(8, '0')}` | `#{b_tag}` | `#{b_index}` | `#{b_offset}` | `#{replace}`"
end



# fully associative

puts "4 assc. sets: no index"
puts "64 bytes in block: 6 bit offset"
addresses.each do |addr|
  b_tag = addr[..-6].to_i(2).to_s(16).rjust(8, '0')
  b_offset = addr[-6..].rjust(6, '0')

  # address tag index offset replace
  puts "`#{addr.to_i(2).to_s(16).rjust(8, '0')}` | `#{b_tag}` | | `#{b_offset}` | `true`"
end

# 2-way set-associative
cache = {
  0 => [nil, nil],
  1 => [nil, nil]
}
  puts "2 sets (2x2): 1 bit index"
  puts "64 bytes in block: 6 bit offset"
addresses.each do |addr|
  b_tag = addr[..-7].to_i(2).to_s(16).rjust(7, '0')
  b_index = addr [-7..-7]
  b_offset = addr[-6..].rjust(6, '0')

  replace = !!cache[b_index]
  cache[b_index] = addr

  # address tag index offset replace
  puts "`#{addr.to_i(2).to_s(16).rjust(8, '0')}` | `#{b_tag}` | `#{b_index}` | `#{b_offset}` | `#{replace}`"
end
