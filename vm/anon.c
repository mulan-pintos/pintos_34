/* anon.c: Implementation of page for non-disk image (a.k.a. anonymous page).
  anon.c : 디스크가 아닌 이미지에 대한 페이지 구현(익명 페이지라고도 함) */

#include "vm/vm.h"
#include "devices/disk.h"

/* DO NOT MODIFY BELOW LINE */
static struct disk *swap_disk;
static bool anon_swap_in (struct page *page, void *kva);
static bool anon_swap_out (struct page *page);
static void anon_destroy (struct page *page);

/* DO NOT MODIFY this struct */
static const struct page_operations anon_ops = {
	.swap_in = anon_swap_in,
	.swap_out = anon_swap_out,
	.destroy = anon_destroy,
	.type = VM_ANON,
};

// for anon_init
struct bitmap *swap_table;
const size_t SECTORS_PER_PAGE = PGSIZE / DISK_SECTOR_SIZE;

/* Initialize the data for anonymous pages
익명 페이지에 대한 데이터 초기화 */
void
vm_anon_init (void) {
	swap_disk = disk_get(1, 1);
    size_t swap_size = disk_size(swap_disk) / SECTORS_PER_PAGE;
    swap_table = bitmap_create(swap_size);
}

/* Initialize the file mapping 
파일 매핑 초기화*/
bool
anon_initializer (struct page *page, enum vm_type type, void *kva) {
	/* Set up the handler */
	
	// struct uninit_page *uninit = &page->uninit;
    // memset(uninit, 0, sizeof(struct uninit_page));
	
	page->operations = &anon_ops;

	struct anon_page *anon_page = &page->anon;
	
	// anon_page->swap_index = -1;
}

/* Swap in the page by read contents from the swap disk. */
static bool
anon_swap_in (struct page *page, void *kva) {
	struct anon_page *anon_page = &page->anon;
// swap disk에서 anon page로 스왑

// struct page에 있는 index = 데이터 위치
// swap table 업데이트

}

/* Swap out the page by writing contents to the swap disk. */
static bool
anon_swap_out (struct page *page) {
	struct anon_page *anon_page = &page->anon;
// 메모리에서 디스크로 내용 복사 -> anon page를 스왑 디스크로 보냄

// 스왑 테이블에서 사용 가능한 스왑 슬롯 찾기 (슬롯 == 1 page)

// 데이터가 담긴 페이지를 슬롯에 복사

// 데이터의 위치를 페이지 구조체에 저장


}

/* Destroy the anonymous page. PAGE will be freed by the caller. */
static void
anon_destroy (struct page *page) {
	struct anon_page *anon_page = &page->anon;
}
