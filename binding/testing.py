import file_organization as fo

def create_anime_record_and_display():
    record = fo.AnimeRecord()
    record.anime_id = 1
    record.episodes = 2
    record.key = 21.2
    record.members = 12
    record.setName("Bungou Stray Dogs")
    record.setGenre("mystery")
    record.setType("idks")
    record.show()

    print("Print Anime Record's attributes one by one")
    print("id: ", record.anime_id)
    print("episodes: ", record.episodes)
    print("key:", record.key)
    print("members: ", record.members)
    print("name", record.getName())
    print("genre: ", record.getGenre())
    print("type: ", record.getType())

    

def create_app_record_and_display():
    record = fo.AppRecord()
    record.rating = 5.0
    record.rating_count = 783.1
    record.minimum_installs = 123.456
    record.maximum_installs = 456.789
    record.free = False
    record.price = 12.34
    record.setApp_name("Webtoon")
    record.setApp_id("1234567890")
    record.setCategory("Entertainment")
    record.setInstalls("12M")
    record.setCurrency("jajaj")
    record.setSize("12MB")
    record.show()


create_app_record_and_display()