namespace cpp server

struct SerialisedNode {
    1: i64 index,
    2: i32 level,
    3: binary key
}

service SSEService {
    oneway void add_entries(1:string key, 2:string val, 3:i32 flag),
    list<i32> search(1:i32 st, 2:string Kw, 3:list<SerialisedNode> node_list,
                            4:i32 level),
    list<i32> verify(1:string D, 2:string Kw)
}