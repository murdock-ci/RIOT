/*
 * Copyright (C) 2017 Freie Universität
 *
 * Distributed under terms of the LGPLv2.1 license (see LICENSE)
 */

$("#MSearchBox").first(function(index) {
    $(this).remove();
});

$("#navrow1 ul.tablist li").each(function(index) {
    var element = $(this).detach();
    if (element.text().trim() == "") {
        return;
    }
    if (element.attr("class") == "current") {
        element.attr("class", "active");
    }
    $("#riot-navlist").append(element);
});
$("#navrow1").remove()

var prev_element = $("nav.navbar")
for (i = 2; i < 5; i++) {
    $("#navrow" + i + " ul.tablist").each(function(index) {
        var element = $(this).detach();
        element.attr("class", "nav nav-tabs nav-tabs-regs");
        element.children("li.current").each(function(i) {
            $(this).attr("class", "active");
        });
        prev_element.after(element);
        prev_element = element;
    });
    $("#navrow" + i)
}

$("img").not("#brand-logo > img").not(".footer").addClass("img-responsive")

if ($(window).width() < 750) {
    $("#side-nav").remove()
}
