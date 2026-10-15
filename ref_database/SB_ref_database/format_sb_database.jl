# Julia script to transform the STIX11.jld2 file into the format of the SB_ref_database.h file
# This reads the data exported as 

using JLD2
using DataFrames
using JSON3

include("functions_ss.jl")

sb_ver = "sb11"

if sb_ver == "sb11"
    data = read_data("stx11_data.json")
elseif sb_ver == "sb21"
    data = read_data("stx21_data.json")
elseif sb_ver == "sb24"
    data = read_data("stx24_data.json")
end

# sb24 carries the SLB2022/24 property modifiers in their own file; the older databases do not
mods = sb_ver == "sb24" ? Dict{String,Vector{Float64}}(String(k)=>Float64.(v) for (k,v) in
                           pairs(JSON3.read(read("stx24_modifiers.json",String))) if k != :_comment) :
                          Dict{String,Vector{Float64}}()
out = format_em(data, mods)
print(out)