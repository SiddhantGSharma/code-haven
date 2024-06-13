<?php

function pd_display_partners_list() {
    global $wpdb;
    $current_date = current_time('Y-m-d');
    $table_name = 'jerseystem_partners';

    $where_clause = "WHERE LOWER(`inactive`) = 'no' AND %s BETWEEN start_date AND end_date";

    // Retrieve partner categories
    $categories = $wpdb->get_col("SELECT DISTINCT partner_category FROM jerseystem_partners ORDER BY partner_category");

    // Check if there are any partners
    if (!$categories) {
        return '<p>No partners found.</p>';
    }

    // Check if a category is selected
    $selected_category = isset($_GET['partner_category']) ? sanitize_text_field($_GET['partner_category']) : '';

    // Create menu bar with categories
    $menu_bar = '<div id="partner-category-menu" style="margin-bottom: 20px;">';
    $menu_bar .= '<button onclick="filterPartners(\'\')">Show All</button>';
    foreach ($categories as $category) {
        $menu_bar .= sprintf(
            '<button onclick="filterPartners(\'%s\')">%s</button>',
            esc_attr($category),
            esc_html($category)
        );
    }
    $menu_bar .= '</div>';

    $params = array($current_date);

    if ($selected_category) {
        $where_clause .= " AND partner_category = %s";
        $params[] = $selected_category;
    }

    $partners = $wpdb->get_results(
        $wpdb->prepare(
            "SELECT `id`, partner_name, partner_logo, sponsor_program, partner_description 
            FROM $table_name 
            $where_clause
            ORDER BY partner_category, partner_name",
            $params
        )
    );

    if (!$partners) {
        return $menu_bar . '<p>No partners found.</p>';
    }

    // Generate partners list
    $partner_details_page_id = 290;
    $partner_details_page_url = get_permalink($partner_details_page_id);

    $output = $menu_bar;
    $output .= '<ul id="partners-list" style="list-style-type:none">';
    foreach ($partners as $partner) {
        $url = esc_url(add_query_arg('partner_id', $partner->id, $partner_details_page_url));
        $partner_name = esc_html($partner->partner_name);
        $partner_description = esc_html($partner->partner_description);

        // Get the first line of the description
        $first_line = strtok($partner_description, "\n");
        if (strlen($first_line) > 100) {
            $first_line = substr($first_line, 0, 100) . '...';
        }

        $base_height = 100;

        if (!empty($partner->partner_logo)) {
            $logo_url = esc_url($partner->partner_logo);
            $scale_factor = (int)$partner->sponsor_program;
            $image_height = $base_height * ($scale_factor * 0.5);
            $output .= sprintf(
                '<li style="position: relative;">
                    <a href="%s"><img src="%s" alt="%s" style="height:%dpx"></a>
                    <div class="partner-popup" style="display: none; position: absolute; top: 110%%; left: 50%%; transform: translateX(-50%%); background: white; border: 1px solid #ccc; padding: 10px; z-index: 1000;">
                        <strong>%s</strong><br>
                        %s
                    </div>
                </li>',
                $url,
                $logo_url,
                $partner_name,
                $image_height,
                $partner_name,
                $first_line
            );
        } else {
            $output .= sprintf(
                '<li style="position: relative;">
                    <a href="%s">%s</a>
                    <div class="partner-popup" style="display: none; position: absolute; top: 110%%; left: 50%%; transform: translateX(-50%%); background: white; border: 1px solid #ccc; padding: 10px; z-index: 1000;">
                        <strong>%s</strong><br>
                        %s
                    </div>
                </li>',
                $url,
                $partner_name,
                $partner_name,
                $first_line
            );
        }
    }
    $output .= '</ul>';

    // Add JavaScript to handle hover effects and category selection
    $output .= <<<SCRIPT
    <script>
    function filterPartners(category) {
        const urlParams = new URLSearchParams(window.location.search);
        if (category) {
            urlParams.set('partner_category', category);
        } else {
            urlParams.delete('partner_category');
        }
        const newUrl = '?' + urlParams.toString();
        history.replaceState({}, '', newUrl);

        // Fetch new content based on the updated URL
        fetch(newUrl, {
            method: 'GET',
            headers: {
                'Content-Type': 'text/html',
                'X-Requested-With': 'XMLHttpRequest'
            }
        })
        .then(response => response.text())
        .then(html => {
            const parser = new DOMParser();
            const doc = parser.parseFromString(html, 'text/html');
            const newList = doc.querySelector('#partners-list');
            document.querySelector('#partners-list').replaceWith(newList);

            const newMenu = doc.querySelector('#partner-category-menu');
            document.querySelector('#partner-category-menu').replaceWith(newMenu);

            attachHoverEvents();
        })
        .catch(error => console.error('Error:', error));
    }

    function attachHoverEvents() {
        const partners = document.querySelectorAll('#partners-list li');
        partners.forEach(partner => {
            partner.addEventListener('mouseenter', () => {
                partner.querySelector('.partner-popup').style.display = 'block';
            });
            partner.addEventListener('mouseleave', () => {
                partner.querySelector('.partner-popup').style.display = 'none';
            });
        });
    }

    document.addEventListener('DOMContentLoaded', attachHoverEvents);
    </script>
    SCRIPT;

    return $output;
}
add_shortcode('partners-list', 'pd_display_partners_list');

// Shortcode to display individual partner details
function pd_display_partner_details() {
    if (!isset($_GET['partner_id'])) {
        return '<p>Invalid partner ID.</p>';
    }

    global $wpdb;
    $partner_id = intval($_GET['partner_id']);
    $table_name = 'jerseystem_partners';

    $partner = $wpdb->get_row(
        $wpdb->prepare(
            "SELECT partner_logo, partner_name, partner_description 
            FROM $table_name 
            WHERE `id` = %d",
            $partner_id
        )
    );

    if (!$partner) {
        return '<p>Partner not found.</p>';
    }

    $output = '<img src="' . esc_html($partner->{'partner_logo'}) . '" alt="' . esc_html($partner->{'partner_name'}) . '" style="height:50px">';
    $output .= '<h2>' . esc_html($partner->{'partner_name'}) . '</h2>';
    $output .= '<p>' . esc_html($partner->{'partner_description'}) . '</p>';

    return $output;
}
add_shortcode('partner-details', 'pd_display_partner_details');
