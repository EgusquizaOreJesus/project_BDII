import build.file_organization as fo  


def create_video_record_and_display():
    record = fo.VideoRecord()
    record.views = 20
    record.likes = 50
    record.dislikes = 10
    record.comment_count = 10
    record.setKey("111222333")
    record.setTitle("Gatos graciosos")
    record.setChannel_title("Gatitos")
    print(record.show())

def create_app_record_and_display():
    record = fo.AppRecord()
    record.rating = 5.0
    record.rating_count = 783.1
    record.minimum_installs = 123.456
    record.maximum_installs = 456.789
    record.free = False
    record.price = 12.34
    record.setApp_name("Webtoon")
    record.setKey("1234567890")
    record.setCategory("Entertainment")
    record.setInstalls("12M")
    record.setCurrency("jajaj")
    record.setSize("12MB")
    print(record.show())

create_video_record_and_display()
create_app_record_and_display()